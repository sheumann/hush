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
