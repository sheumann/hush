/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) tons of folks.  Tracking down who wrote what
 * isn't something I'm going to worry about...  If you wrote something
 * here, please feel free to acknowledge your work.
 *
 * Based in part on code from sash, Copyright (c) 1999 by David I. Bell
 * Permission has been granted to redistribute this code under GPL.
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */

#include "busybox.h"

void FAST_FUNC bb_show_usage(void)
{
	xfunc_die();
}

/* check if u is member of group g */
int ingroup(uid_t u, gid_t g)
{
	struct group *grp = getgrgid(g);
	if (grp) {
		char **mem;
		for (mem = grp->gr_mem; *mem; mem++) {
			struct passwd *pwd = getpwnam(*mem);
			if (pwd && (pwd->pw_uid == u))
				return 1;
		}
	}
	return 0;
}
