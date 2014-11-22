#include <libbb.h>

#ifdef __GNO__
/* We use our own implementations of execv* because we need to
 * do some things that libc's version doesn't:
 * 1) Quote arguments containing spaces or tabs
 * 2) Execute shell scripts in a shell (TODO)
 */
 
 /* Note that this expects environ to be initialized.  
  * Also, if envp != environ, it will update the environment of this process
  * (and the one it's forked from, if any, unless there's been an environPush).
  */
int execve(const char *path, char *const *argv, char *const *envp)
{
	int result;
	char *args;
	
	size_t argslen = 0;
	int i;
	char *nextarg;
	
	for (i = 0; argv[i] != NULL; i++) {
		/* 3 bytes extra for space (or terminating 0) and possible quotes */
		argslen += strlen(argv[i]) + 3;
	}
	
	nextarg = args = malloc(argslen);
	if (args == NULL) {
		free(path);
		errno = ENOMEM;
		return -1;
	}
	
	/* Copy arguments into a single string, quoting ones that contain spaces
	 * or tabs.  This approach won't give the right result in all cases
	 * (e.g. when the argument starts with " or contains both spaces and "),
	 * but it's about the best we can do, since we're dependent on the 
	 * argument-parsing code in the target program.
	 */
	for (i = 0; argv[i] != NULL; i++) {
		bool has_space = (strpbrk(argv[i], " \t") != NULL);

		if (has_space)
			*nextarg++ = '"';
		strcpy(nextarg, argv[i]);
		nextarg += strlen(argv[i]);
		if (has_space)
			*nextarg++ = '"';
		*nextarg++ = ' ';
	}
	*(nextarg - 1) = 0;
	
	/* Clear environment and then rebuild it, if necessary. */
	if (envp != environ) {
		while (environ[0] != NULL)
			bb_unsetenv(environ[0]);
		if (buildEnv(envp) != 0)
			return -1;
	}
	
	result = _execve(path, args);
	
	/* error case */
	free(args);
	return result;
} 

int execv(const char *path, char *const *argv)
{
	return execve(path, argv, environ);
}
 
int execvp(const char *file, char *const *argv)
{
	int result;
	char *path;
	
	path = buildPath(file);
	if (path == NULL) {
		errno = ENOENT;
		return -1;
	}
	
	result = execve(path, argv, environ);
	
	/* error case */
	free(path);
	return result;
}
#endif
