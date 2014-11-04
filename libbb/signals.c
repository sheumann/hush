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

void FAST_FUNC bb_signals(int sigs, void (*f)(int))
{
	int sig_no = 0;
	int bit = 1;

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
	raise(sig);
	_exit(sig | 128); /* Should not reach it */
}
