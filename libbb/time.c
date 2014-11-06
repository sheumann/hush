/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 2007 Denys Vlasenko
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
#include "libbb.h"

#ifndef __GNO__

static char* strftime_fmt(char *buf, unsigned len, time_t *tp, const char *fmt)
{
	time_t t;
	if (!tp) {
		tp = &t;
		time(tp);
	}
	/* Returns pointer to NUL */
	return buf + strftime(buf, len, fmt, localtime(tp));
}

char* FAST_FUNC strftime_HHMMSS(char *buf, unsigned len, time_t *tp)
{
	return strftime_fmt(buf, len, tp, "%H:%M:%S");
}

char* FAST_FUNC strftime_YYYYMMDDHHMMSS(char *buf, unsigned len, time_t *tp)
{
	return strftime_fmt(buf, len, tp, "%Y-%m-%d %H:%M:%S");
}

#endif

unsigned long FAST_FUNC monotonic_sec(void)
{
	return time(NULL);
}
