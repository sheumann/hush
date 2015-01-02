/* vi: set sw=4 ts=4: */
/*
 * Utility routines.
 *
 * Copyright (C) 2008 Rob Landley <rob@landley.net>
 * Copyright (C) 2008 Denys Vlasenko <vda.linux@googlemail.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
#include "libbb.h"

int32_t FAST_FUNC read_key(int fd, char *buffer, int timeout)
{
	struct pollfd pfd;
	int n;
	int count;
	unsigned char c;

	pfd.fd = fd;
	pfd.events = POLLIN;

	buffer++; /* saved chars counter is in buffer[-1] now */

	errno = 0;
	n = (unsigned char)buffer[-1];
	if (n == 0) {
		/* If no data, wait for input.
		 * If requested, wait TIMEOUT ms. TIMEOUT = -1 is useful
		 * if fd can be in non-blocking mode.
		 */
		/* On GNO, skip the poll if timeout == -1.
		 * It's pointless, since non-blocking mode isn't implemented,
		 * and it seems to hang in certain cases (e.g. where another 
		 * process was interrupted while reading from the terminal).
		 * This may be due to bugs in the underlying select() implementation.
		 */
#ifndef __GNO__
		if (timeout >= -1) 
#else
		if (timeout >= 0) 
#endif
		{
			if (safe_poll(&pfd, 1, timeout) == 0) {
				/* Timed out */
				errno = EAGAIN;
				return -1;
			}
		}
		/* It is tempting to read more than one byte here,
		 * but it breaks pasting. Example: at shell prompt,
		 * user presses "c","a","t" and then pastes "\nline\n".
		 * When we were reading 3 bytes here, we were eating
		 * "li" too, and cat was getting wrong input.
		 */
		n = safe_read(fd, buffer, 1);
		if (n <= 0)
			return -1;
	}

	/* ASCII printable chars presumably don't start escape sequences */
	c = buffer[0];
	if (c >= ' ' && c <= '~') {
		goto return_c;
	}

	/* Loop through known escape sequences */
	for (count = 0; count < n_escape_seqs; count++) {
		struct escape_seq *escseq = &escape_seqs[count];
		
		/* n - position in sequence we did not read yet */
		int i = 0; /* position in sequence to compare */

		while (1) {
			/* So far escape sequence matched up to [i-1] */
			if (escseq->seq[i] == 0) {
				/* Entire seq matched */
				n = 0;
				/* n -= i; memmove(...);
				 * would be more correct,
				 * but we never read ahead that much,
				 * and n == i here. */
				buffer[-1] = 0;
				return escseq->keycode;
			}
			
			if (n <= i) {
				/* Need more chars, read another one if it wouldn't block.
				 * Note that escape sequences come in as a unit,
				 * so if we block for long it's not really an escape sequence.
				 * Timeout is needed to reconnect escape sequences
				 * split up by transmission over a serial console. */
				if (safe_poll(&pfd, 1, 50) == 0) {
					/* No more data!
					 * Can't match this sequence. Keep looping in case
					 * a later one is shorter. */
					goto next_seq;
				}
				errno = 0;
				if (safe_read(fd, buffer + n, 1) <= 0) {
					/* If EAGAIN, then fd is O_NONBLOCK and poll lied:
					 * in fact, there is no data. */
					if (errno != EAGAIN) {
						/* otherwise: it's EOF/error */
						buffer[-1] = 0;
						return -1;
					}
					goto got_all;
				}
				n++;
			}
			if (buffer[i] != (escseq->seq[i] & 0x7f)) {
				/* This seq doesn't match, go to next */
				goto next_seq;
			}
			i++;
		}
 next_seq: ;
	}

	/* We did not find matching sequence.
	 * We possibly read and stored more input in buffer[] by now.
	 * n = bytes read.
	 */
 got_all:
	if (n <= 0) {
		buffer[-1] = 0;
		return -1;
	}

	/* Returning miscellaneous characters may result in unknown 
	 * escape sequences being interpreted as ESC + garbage, but
	 * it's difficult to know what to filter out, so we'll
	 * just return everything for now.
	 */
	c = buffer[0];
 return_c:
	n--;
	if (n)
		memmove(buffer, buffer + 1, n);
	buffer[-1] = n;
	return c;
}

void FAST_FUNC read_key_ungets(char *buffer, const char *str, unsigned len)
{
	unsigned cur_len = (unsigned char)buffer[0];
	if (len > KEYCODE_BUFFER_SIZE-1 - cur_len)
		len = KEYCODE_BUFFER_SIZE-1 - cur_len;
	memcpy(buffer + 1 + cur_len, str, len);
	buffer[0] += len;
}
