/* We use our own version of qsort because ORCA/C's version is recursive and
 * can take up quite a bit of stack space (at least several thousand KB).
 * 
 * This implementation is adapted from (an old version of) musl libc.
 * 
 * Copyright (c) 2005-2011 Rich Felker
 * Originally licensed under the GNU LGPL version 2.1 or later.
 * Licensed under GPLv2 (pursuant to clause 3 of the LGPL version 2.1), 
 * see file LICENSE in this source tree.
 */

#include <stdlib.h>
#include <string.h>

/* A simple heap sort implementation.. only in-place O(nlogn) sort I know. */

#define MIN(a, b) ((a)<(b) ? (a) : (b))

static void swap(char *a, char *b, size_t len)
{
	char tmp;
	char *a_end = a + len;
	while (a != a_end) {
		tmp = *a;
		*a++ = *b;
		*b++ = tmp;
	}
}

static void sift(char *base, size_t root, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	size_t max;

	while (2*root <= nel) {
		max = 2*root;
		if (max < nel && cmp(base+max*width, base+(max+1)*width) < 0)
			max++;
		if (max && cmp(base+root*width, base+max*width) < 0) {
			swap(base+root*width, base+max*width, width);
			root = max;
		} else break;
	}
}

void qsort(void *_base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	char *base = _base;
	size_t i;

	if (!nel) return;
	for (i=(nel+1)/2; i; i--)
		sift(base, i-1, nel-1, width, cmp);
	for (i=nel-1; i; i--) {
		swap(base, base+i*width, width);
		sift(base, 0, i-1, width, cmp);
	}
}
