/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 * Copyright (C) 2006 Rob Landley
 * Copyright (C) 2006 Denys Vlasenko
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include "libbb.h"

/* All known arches use small ints for signals */
smallint bb_got_signal;

int FAST_FUNC sigprocmask_allsigs(int how)
{
	sigset_t set;
	sigfillset(&set);
	return sigprocmask(how, &set, NULL);
}

void FAST_FUNC bb_signals(long int sigs, sig_t f)
{
	int sig_no = 0;
	long int bit = 1;

	while (sigs) {
		if (sigs & bit) {
			sigs -= bit;
			signal(sig_no, f);
		}
		sig_no++;
		bit <<= 1;
	}
}

void FAST_FUNC sig_unblock(int sig)
{
	sigset_t ss;
	sigemptyset(&ss);
	sigaddset(&ss, sig);
	sigprocmask(SIG_UNBLOCK, &ss, NULL);
}

/* Assuming the sig is fatal */
void FAST_FUNC kill_myself_with_sig(int sig)
{
	signal(sig, SIG_DFL);
	sig_unblock(sig);
	signal_parent_to_resume();
	raise(sig);
	_exit(sig | 128); /* Should not reach it */
}

#ifdef __GNO__
/* Include our own version of sigprocmask, because the one in 
 * GNO 2.0.6 libc is broken for SIG_SETMASK and SIG_UNBLOCK cases 
 * (it will never unblock any signals). */
int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	sigset_t oldmask;
	
	if (set) {
		switch (how) {
		case SIG_BLOCK:
			oldmask = sigblock(*set);
			break;
		case SIG_SETMASK:
			oldmask = sigsetmask(*set);
			break;
		case SIG_UNBLOCK:
			oldmask = sigblock(0);
			sigsetmask(oldmask & ~*set);
			break;
		default:
			errno = EINVAL;
			return -1;
		}
	} else if (oset) {
		oldmask = sigblock(0);
	}
	
	if (oset)
		*oset = oldmask;
	
	return 0;
}
#endif
