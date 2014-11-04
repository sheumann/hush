/* vi: set sw=4 ts=4: */
/*
 * ascii-to-numbers implementations for busybox
 *
 * Copyright (C) 2003  Manuel Novoa III  <mjn3@codepoet.org>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include "libbb.h"

#define type long
#define xstrtou xstrtoul
#define xstrto xstrtol
#define xatou xatoul
#define xato xatol
#define xstrtou_(rest) xstrtoul_##rest
#define xstrto_(rest) xstrtol_##rest
#define xatou_(rest) xatoul_##rest
#define xato_(rest) xatol_##rest
#define XSTR_UTYPE_MAX ULONG_MAX
#define XSTR_TYPE_MAX LONG_MAX
#define XSTR_TYPE_MIN LONG_MIN
#define XSTR_STRTOU strtoul
#include "xatonum.tmplt.c"

#if UINT_MAX != ULONG_MAX
static ALWAYS_INLINE
unsigned bb_strtoui(const char *str, char **end, int b)
{
	unsigned long v = strtoul(str, end, b);
	if (v > UINT_MAX) {
		errno = ERANGE;
		return UINT_MAX;
	}
	return v;
}
#define type int
#define xstrtou xstrtou
#define xstrto xstrtoi
#define xatou xatou
#define xato xatoi
#define xstrtou_(rest) xstrtou_##rest
#define xstrto_(rest) xstrtoi_##rest
#define xatou_(rest) xatou_##rest
#define xato_(rest) xatoi_##rest
#define XSTR_UTYPE_MAX UINT_MAX
#define XSTR_TYPE_MAX INT_MAX
#define XSTR_TYPE_MIN INT_MIN
/* libc has no strtoui, so we need to create/use our own */
#define XSTR_STRTOU bb_strtoui
#include "xatonum.tmplt.c"
#endif

/* A few special cases */

int FAST_FUNC xatoi_positive(const char *numstr)
{
	return xatou_range(numstr, 0, INT_MAX);
}

uint16_t FAST_FUNC xatou16(const char *numstr)
{
	return xatou_range(numstr, 0, 0xffff);
}

const struct suffix_mult bkm_suffixes[] = {
	{ "b", 512 },
	{ "k", 1024 },
	{ "m", 1024*1024 },
	{ "", 0 }
};
