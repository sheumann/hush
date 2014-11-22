#include <unistd.h>
#include <errno.h>
#ifdef __GNO__
# include <gno/gno.h>
# include <gno/kvm.h>
#endif

#ifndef __GNO__

int tc_set_to_my_pgrp(int fd)
{
	return tcsetpgrp(fd, getpgrp());
}

#else

int tc_set_to_my_pgrp(int fd)
{
	return tctpgrp(fd, getpid());
}

/* We implement tcsetpgrp by searching the process table for a process
 * in the right pgrp and calling tctpgrp using that process.
 */
int tcsetpgrp(int fd, pid_t pgrp)
{
	kvmt *kvm_context;
	struct pentry *proc_entry;
	int result = 1;

	kvm_context = kvm_open();
	if (kvm_context == NULL)
		return -1;
		
	while ((proc_entry = kvmnextproc(kvm_context)) != NULL) {
		if (proc_entry->pgrp == pgrp) {
			if (tctpgrp(fd, kvm_context->pid) != -1) {
				result = 0;
				break;
			} else {
				result = -1;
			}
		}
	}
	
	kvm_close(kvm_context);
	
	if (result == 1) {	/* if we didn't find a matching process */
		result = -1;
		errno = ESRCH;
	}
	
	return result;
}

#endif
