#include <unistd.h>
#include "libbb.h"

/* Like vfork, but calls fn(arg) in the child instead of returning.
 * This is designed to match the semantics of GNO's fork2 call.
 */
#ifndef __GNO__
pid_t vfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	pid_t pid = vfork();

	if (pid == 0) {
		fn(arg);
	}

	return pid;
}
#else
pid_t vfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	return fork2((fn), 1024, 0, "hush (forked)", 2, (arg));
}
#endif

pid_t xvfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	pid_t pid = vfork_and_run(fn, arg);

	if (pid < 0)
		bb_perror_msg_and_die("vfork");

	return pid;
}
