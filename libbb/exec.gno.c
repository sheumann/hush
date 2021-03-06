/*
 * Copyright (c) 2014 Stephen Heumann
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "libbb.h"
#undef inline
#include <gsos.h>
#include <orca.h>
#include <memory.h>
#include <gno/kvm.h>

#ifdef __GNO__
/* We use our own implementations of execv* because we need to
 * do some things that libc's version doesn't:
 * 1) Quote arguments containing spaces or tabs
 * 2) Execute shell scripts in a shell
 */
 
extern char *hush_exec_path;
 
#define MAX_HASHBANG_LINE 127

/* Allocate memory that is associated with the current process
 * (even if it's been forked), and thus will be deallocated once 
 * it quits or successfully execs.
 */
void *alloc_for_current_process(size_t size)
{
	kvmt *kvm_context;
	struct pentry *proc_entry;
	int userID;
	Handle handle;

	/* Get current user ID from our process entry */
	kvm_context = kvm_open();
	if (kvm_context == NULL)
		return NULL;
	proc_entry = kvmgetproc(kvm_context, getpid());
	userID = proc_entry->userID;
	kvm_close(kvm_context);
	
	handle = NewHandle(size, userID, 0xC008, NULL);
	if (toolerror()) {
		errno = ENOMEM;
		return NULL;
	}
	
	return *handle;
}

/* Deallocate memory allocated by alloc_for_current_process */
void dealloc_for_current_process(void *ptr)
{
	Handle handle;

	if (ptr == NULL)
		return;
	handle = FindHandle(ptr);
	if (handle == NULL) {
		fprintf(stderr, "invalid deallocation detected!\n");
		errno = EINVAL;
		return;
	}
	DisposeHandle(handle);
}
 

/* Build a single args string out of argv, with quoting.
 * Returns the (alloc_for_current_process'd) args string, or NULL on error.
 */
char *build_args(char *const *argv)
{
	char *args;
	size_t argslen = 0;
	int i;
	char *nextarg;
	
	if (argv == NULL || argv[0] == NULL) {
		/* We require non-empty argv, which simplifies a few things. */
		errno = EINVAL;
		return NULL;
	}
	
	for (i = 0; argv[i] != NULL; i++) {
		/* 3 bytes extra for space (or terminating 0) and possible quotes */
		argslen += strlen(argv[i]) + 3;
	}
	
	nextarg = args = alloc_for_current_process(argslen);
	if (args == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	
	/* Copy arguments into a single string, quoting ones that contain spaces
	 * or tabs (or are empty strings).  This approach won't give the right 
	 * result in all cases (e.g. when the argument starts with " or contains 
	 * both spaces and "), but it's about the best we can do, since we're 
	 * dependent on the argument-parsing code in the target program.
	 */
	for (i = 0; argv[i] != NULL; i++) {
		bool has_space = (strpbrk(argv[i], " \t") != NULL || *argv[i] == '\0');

		if (has_space)
			*nextarg++ = '"';
		strcpy(nextarg, argv[i]);
		nextarg += strlen(argv[i]);
		if (has_space)
			*nextarg++ = '"';
		*nextarg++ = ' ';
	}
	*(nextarg - 1) = '\0'; 
	
	return args;
 }
 
/* Note that this expects environ to be initialized.  
 * Also, if envp != environ, it will update the environment of this process
 * (and the one it's forked from, if any, unless there's been an environPush).
 */
int execve(const char *path, char *const *argv, char *const *envp)
{
	char *args = NULL;
	size_t pathlen;
	GSString255Ptr path_gs = NULL;
 	static FileInfoRecGS fileInfoRec;
 	int script_fd;
 	static char hashbang_line[MAX_HASHBANG_LINE + 1];
 	int hashbang_line_len;
 	char *interpreter_cmd;
 	int interpreter_arg_offset;
 	char *interpreter_arg = NULL;
 	int narg, i;
 	char **new_argv;
	
	args = build_args(argv);
	if (args == NULL)
		goto error_ret;
	
	/* Clear environment and then rebuild it, if necessary. */
	if (envp != environ) {
		while (environ[0] != NULL)
			bb_unsetenv(environ[0]);
		if (buildEnv(envp) != 0)
			goto error_ret;
	}

	/* Check that the file exists before we try to _execve it.
	 * We must do this now, because if we give _execve a path on 
	 * a non-existent volume, it will put up an unescapable GS/OS 
	 * dialog asking for that volume to be inserted. */
	pathlen = strlen(path);
	path_gs = malloc(strlen(path) + offsetof(GSString255, text) + 1);
	if (path_gs == NULL) {
		errno = ENOMEM;
		goto error_ret;
	}
	strcpy(path_gs->text, path);
	path_gs->length = pathlen;
	fileInfoRec.pCount = 4;
	fileInfoRec.pathname = path_gs;
	GetFileInfoGS(&fileInfoRec);
	if (toolerror()) {
		errno = ENOENT;
		goto error_ret;
	}
	free(path_gs);
	path_gs = NULL;
	
	/* This will close the close-on-exec fds even if the exec 
	 * ultimately fails.  This should be OK for our uses, because
	 * hush just prints an error message and exits in those cases. */
	close_cloexec_fds();
	
	_execve(path, args);
	
	dealloc_for_current_process(args);
	args = NULL;
	
	/* If _execve kernel call failed, consider trying to execute 
	 * the file as a script. If it's not an EXEC file, error out. */
	if (fileInfoRec.fileType != 0xB0 || fileInfoRec.auxType != 0x0006) {
		errno = EACCES;
		goto error_ret;
	}
	
	script_fd = open(path, O_RDONLY);
	if (script_fd == -1)
		goto error_ret;
	hashbang_line_len = read(script_fd, hashbang_line, MAX_HASHBANG_LINE);
 	close(script_fd);
	if (hashbang_line_len < 2)
		goto error_ret;
	hashbang_line[hashbang_line_len] = 0;
	if (hashbang_line[0] != '#' || hashbang_line[1] != '!') {
		/* We use ourselves as the interpreter if the script doesn't specify */
		interpreter_cmd = hush_exec_path;
		goto exec_interpreter;
	}
	
	/* Parse an interpreter specification of the form "#! interpreter [arg]" */
	interpreter_cmd = &hashbang_line[2];
	while (*interpreter_cmd == ' ' || *interpreter_cmd == '\t')
		interpreter_cmd++;
	hashbang_line_len = strcspn(interpreter_cmd, "\r\n");
	if (interpreter_cmd[hashbang_line_len] == '\0') {
		/* Hashbang line was too long */
		errno = E2BIG;
		goto error_ret;
	}
	interpreter_cmd[hashbang_line_len] = '\0';
	if (interpreter_cmd[0] == '\0') {
		/* interpreter command can't be empty string */
		errno = ENOEXEC;
		goto error_ret;
	}
	
	interpreter_arg_offset = strcspn(interpreter_cmd, " \t");
	if (interpreter_cmd[interpreter_arg_offset] != '\0') {
		interpreter_cmd[interpreter_arg_offset] = '\0';
		interpreter_arg = &interpreter_cmd[interpreter_arg_offset + 1];
		while (*interpreter_arg == ' ' || *interpreter_arg == '\t')
			interpreter_arg++;
		if (interpreter_arg[0] == '\0')
			interpreter_arg = NULL;
	}
	
 exec_interpreter:
 	narg = 3;	/* interpreter, script file, and final NULL */
 	if (interpreter_arg);
 		narg++;
 	for (i = 1; argv[i] != NULL; i++) {
 		narg++;
 	}
 	
 	new_argv = malloc(narg * sizeof(*new_argv));
 	if (new_argv == NULL) {
 		errno = ENOMEM;
 		goto error_ret;
 	}
 	
 	narg = 0;
 	new_argv[narg++] = interpreter_cmd;
 	if (interpreter_arg)
 		new_argv[narg++] = interpreter_arg;
 	new_argv[narg++] = path;
 	for (i = 1; argv[i] != NULL; i++) {
 		new_argv[narg++] = argv[i];
 	}
 	new_argv[narg] = NULL;
	
	args = build_args(new_argv);
	free(new_argv);
	if (args == NULL)
		goto error_ret;
	
	_execve(interpreter_cmd, args);
	
 error_ret:
	/* error case */
	free(path_gs);
	dealloc_for_current_process(args);
	return -1;
} 

int execv(const char *path, char *const *argv)
{
	return execve(path, argv, environ);
}
 
int execvp(const char *file, char *const *argv)
{
	int result;
	char *path, *path2;
	
	if (!strpbrk(file, "/:")) {
		path = buildPath(file);
		if (path == NULL) {
			errno = ENOENT;
			return -1;
		}
	
		/* Move path to memory that will be freed following _execve */
		path2 = alloc_for_current_process(strlen(path) + 1);
		if (path2 == NULL) {
			free(path);
			errno = ENOMEM;
			return -1;
		}
		strcpy(path2, path);
		free(path);
	} else {
		path2 = (char *)file;
	}
	
	result = execve(path2, argv, environ);
	
	/* error case */
	if (path2 != file) {
		dealloc_for_current_process(path2);
	}
	return result;
}
#endif
