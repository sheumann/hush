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

# include <signal.h>
# include <gno/kvm.h>
# include <orca.h>

/* Turn off all ORCA/C stack repair code to avoid corruption. */
# ifdef __ORCAC__
#  pragma optimize 72
# endif

# pragma databank 1
void fork_thunk(void (*fn)(void*) NORETURN, void *arg, long sigmask) {
	sigsetmask(sigmask);
	fn(arg);
}
# pragma databank 0

const char * forked_child_name = "hush (forked)";

pid_t vfork_and_run(void (*fn)(void*) NORETURN, void *arg) {
	/* GNO's fork2 call will return immediately and allow the parent and 
	 * child processes to execute concurrently using the same memory
	 * space.  To prevent them stomping on each other, we want to get
	 * behavior like a traditional vfork() implementation, where the
	 * parent blocks until the child terminates or execs.
	 *
	 * Our approach is to check the process tables to make sure the
	 * child has actually finished or exec'd.  If not, we loop and try again.
	 * We can't just rely on the fact that the child signaled us, because
	 * it may still be running in libc's implementation of exec*.
	 */
	
	long oldmask;
	bool environPushed;
	pid_t pid;
	kvmt *kvm_context;
	struct pentry *proc_entry;
	bool done = 0;
	
	/* Isolate child process's environment from parent */
	environPushed = !environPush();
	
	/* Block all signals for now */
	oldmask = sigblock(-1);
	
	pid = fork2(fork_thunk, 1024, 0, forked_child_name, 
				(sizeof(fn) + sizeof(arg) + sizeof(oldmask) + 1) / 2, 
				fn, arg, oldmask);
	if (pid < 0) 
		goto ret;
	
	while (!done) {
		/* Wait for ~100 ms.  If procsend worked, the child could send a 
		 * message with it to end the waiting earlier, but this isn't 
		 * possible in GNO 2.0.6 because procsend is broken.  This isn't
		 * too big an issue, since 100ms isn't very long to wait anyhow. */
		procrecvtim(1);
		
		/* Check if the child is really dead or forked by inspecting
		 * the kernel's process entry for it. */
		kvm_context = kvm_open();
		if (kvm_context == NULL)
			break;
		proc_entry = kvmgetproc(kvm_context, pid);
		if (proc_entry == NULL 
			|| (proc_entry->args != NULL 
				&& strcmp(forked_child_name, proc_entry->args + 8) != 0))
			done = 1;
		kvm_close(kvm_context);
	}
	
ret:
	sigsetmask(oldmask);
	if (environPushed)
		environPop();
	return pid;
}

#endif
