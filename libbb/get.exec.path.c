/*
 * Code to get the full path of the currently-running executable on various platforms.
 * 
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

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

/* This should return a pointer that can be deallocated with free() when done,
 * or NULL on failure. */
char *get_exec_path(void);


#ifdef __appleiigs__

#include <gsos.h>
#include <orca.h>

/* Prefix 9 is set to the directory containing the executable at launch.
 * We append the result of a GetName call to this to get the full pathname.
 */
char *get_exec_path(void) {
	PrefixRecGS prefixRec;  
	GetNameRecGS nameRec;
	ResultBuf255 *prefix, *name;
	char *result;
	
	prefix = malloc(sizeof(ResultBuf255));
	if (prefix == NULL)
		return NULL;
	prefixRec.pCount = 2;
	prefixRec.prefixNum = 9; /* Info for prefix 9 */
	prefixRec.buffer.getPrefix = prefix;
	prefix->bufSize = sizeof(ResultBuf255);
	
	GetPrefixGS(&prefixRec);
	if (toolerror()) {
		free(prefix);
		return NULL;
	}
	
	name = malloc(sizeof(ResultBuf255));
	if (name == NULL) {
		free(prefix);
		return NULL;
	}
	nameRec.pCount = 1;
	nameRec.dataBuffer = name;
	name->bufSize = sizeof(ResultBuf255);
	
	GetNameGS(&nameRec);
	if (toolerror()) {
		free(prefix);
		free(name);
		return NULL;
	}
	
	result = malloc(prefix->bufString.length + name->bufString.length + 1);
	if (result == NULL) {
		free(prefix);
		free(name);
		return NULL;
	}
	
	memcpy(result, prefix->bufString.text, prefix->bufString.length);
	memcpy(result + prefix->bufString.length, name->bufString.text, name->bufString.length);
	result[prefix->bufString.length + name->bufString.length] = 0;
	free(prefix);
	free(name);
	return result;
}

#elif defined(__APPLE__) && defined(__MACH__)

#include <mach-o/dyld.h>

char *get_exec_path(void) {
	char *buf;
	uint32_t bufsize = 256;
	int result;
	
	while (1) {
		buf = malloc(bufsize);
		if (buf == NULL)
			return NULL;
	
		result = _NSGetExecutablePath(buf, &bufsize);
		if (result == 0)
			return buf;
		
		free(buf);
	}
}

#elif defined(__linux__)

char *get_exec_path(void) {
	return strdup("/proc/self/exe");
}

#elif defined(BSD)

char *get_exec_path(void) {
	/* Works on some BSDs but not all -- hope for the best! */
	return strdup("/proc/curproc/exe");
}

#else

char *get_exec_path(void) {
	return NULL;
}

#endif