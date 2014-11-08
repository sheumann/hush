/*
 * Code to get the full path of the currently-running executable on various platforms.
 *
 * By Stephen Heumann
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

/* Prefix 1 is set to the directory containing the executable at launch.
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
	prefixRec.prefixNum = 1; /* Info for prefix 1 */
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