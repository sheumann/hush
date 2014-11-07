/* Emulate waitpid on systems that just have wait.
   Adapted from code in GNU Diffutils 2.8.1, with 
   enhancements and changes for GNO by Stephen Heumann.
   
   Copyright (C) 1994, 1995, 1998, 1999 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

#ifdef __GNO__
typedef union wait wait_status_t;
#else
typedef int wait_status_t;
#endif

#ifndef __GNO__
#define alarm10(tenths) ualarm((tenths) * 10000, 0)
#endif

#define WAITPID_CHILDREN 8
static pid_t waited_pid[WAITPID_CHILDREN];
static int waited_status[WAITPID_CHILDREN];

static int ran_sighandler;


#ifdef __ORCAC__
# pragma databank 1
#endif

#ifdef __GNO__
static void handle_alarm(int sig, int code)
#else
static void handle_alarm(int sig)
#endif
{
	/* We're racing with the call to wait().  In case we get the
	 * signal before it starts, arrange to send another one.
	 */
	alarm10(2);
	ran_sighandler = 1;
}

#ifdef __ORCAC__
# pragma databank 0
#endif

pid_t waitpid_emul (pid_t pid, int *stat_loc, int options)
{
	int i;
	pid_t p;

	if (pid == -1 || 0 < pid) {
		/* If we have already waited for this child, return it immediately. */
		for (i = 0;  i < WAITPID_CHILDREN;  i++) {
			p = waited_pid[i];
			if (p && (p == pid || pid == -1)) {
				waited_pid[i] = 0;
				goto success;
			}
		}

		/* The child has not returned yet; wait for it, accumulating status. */
		for (i = 0;  i < WAITPID_CHILDREN;  i++) {
			if (! waited_pid[i]) {
				sig_t prev_sig;
				int prev_errno;
				
				if (options & WNOHANG) {
					/* Arrange for a signal to interrupt the wait to simulate
					 * non-blocking semantics.  This might cause spurious 
					 * failures to detect finished children if the wait() call
					 * is slow, but this is about the best we can do.
					 */
					prev_errno = errno;
					ran_sighandler = 0;
					prev_sig = signal(SIGALRM, handle_alarm);
					alarm10(2);
				}
			
				p = wait ((wait_status_t*)&waited_status[i]);
				
				if (options & WNOHANG) {
					int wait_errno = errno;
					alarm10(0);
					signal(SIGALRM, prev_sig);
					
					if (p == -1 && wait_errno == EINTR && ran_sighandler) {
						/* Assume we were interrupted by our alarm, so treat this
						 * as a successful return with no finished child found. */
						errno = prev_errno;
						return 0;
					}
				}
				
				if (p < 0)
					return p;
				if (p == pid || pid == -1)
					goto success;
				waited_pid[i] = p;
			}
		}
	}

	/* We cannot emulate this wait call, e.g. because of too many children.  */
	errno = EINVAL;
	return -1;

success:
	if (stat_loc)
		*stat_loc = waited_status[i];
	return p;
}

pid_t wait_emul (int *stat_loc)
{
	return waitpid_emul(-1, stat_loc, 0);
}
