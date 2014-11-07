#include <unistd.h>
#include "libbb.h"

pid_t xvfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	pid_t pid = vfork_and_run(fn, arg);

	if (pid < 0)
		bb_perror_msg_and_die("vfork");

	return pid;
}

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

/* Turn off all ORCA/C stack repair code to avoid corruption. */
#ifdef __ORCAC__
# pragma optimize 72
#endif

pid_t vfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	/* GNO's fork2 call will return immediately and allow the parent and 
	 * child processes to execute concurrently using the same memory
	 * space.  To prevent them stomping on each other, we want to get
	 * behavior like a traditional vfork() implementation, where the
	 * parent blocks until the child terminates or execs.
	 *
	 * Our approach will be to have the child send SIGUSR2 to the parent
	 * just before it terminates or execs, and block waiting for that here.
	 * 
	 * It's tempting to use waitpid().  That would have the advantage of 
	 * catching cases where the child process terminates abruptly, but 
	 * GNO's implementation of waitpid() is a wrapper around wait() and 
	 * therefore is buggy: it may swallow the information about termination 
	 * of other child processes (ones that forked earlier and have already 
	 * exec'd), which we want to avoid.
	 */
	
	long oldmask;
	pid_t pid;
	
	/* Block all signals for now */
	oldmask = sigblock(-1);
	
	pid = fork2((fn), 1024, 0, "hush (forked)", 2, (arg));
	
	/* Now wait until we get SIGUSR2 */
	sigpause(~sigmask(SIGUSR2));
	
	/* Restore original signal mask */
	sigsetmask(oldmask);
	
	return pid;
}

#endif
