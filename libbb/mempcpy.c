#include <string.h>

void *mempcpy(void *dst, const void *src, size_t len)
{
	return (char *) memcpy(dst, src, len) + len;
}
