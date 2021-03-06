/* vi: set sw=4 ts=4: */
/*
 * Busybox main internal header file
 *
 * Based in part on code from sash, Copyright (c) 1999 by David I. Bell
 * Permission has been granted to redistribute this code under GPL.
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
#ifndef LIBBB_H
#define LIBBB_H 1

#include "platform.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#if defined __UCLIBC__ /* TODO: and glibc? */
/* use inlined versions of these: */
# define sigfillset(s)    __sigfillset(s)
# define sigemptyset(s)   __sigemptyset(s)
# define sigisemptyset(s) __sigisemptyset(s)
#endif
#ifdef __GNO__
/* Redefine these because GNO 2.0.6 definitions only support signals up to 16 */
# undef sigaddset
# undef sigdelset
# undef sigismember
# define sigaddset(set, signo)   (*(set) |= (sigset_t)1 << ((signo) - 1), 0)
# define sigdelset(set, signo)   (*(set) &= ~((sigset_t)1 << ((signo) - 1)), 0)
# define sigismember(set, signo) ((*(set) & ((sigset_t)1 << ((signo) - 1))) != 0)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
/* There are two incompatible basename's, let's not use them! */
/* See the dirname/basename man page for details */
#undef basename
#define basename dont_use_basename
#include "poll.h"
/* Don't include termios.h in GNO because termios isn't actually implemented,
 * but the header has a define for ECHO that conflicts with the one needed for
 * the ioctls that we use instead.  Do define USE_OLD_TTY before including
 * <sys/ioctl.h>, which is needed to get defines for those ioctls.
 */
#ifndef __GNO__
# include <termios.h>
#else
# define USE_OLD_TTY
#endif
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#ifndef major
# include <sys/sysmacros.h>
#endif
#include <sys/wait.h>
#ifdef __GNO__
/* Fix definition of WEXITSTATUS to support values with the high bit set. */
# undef WEXITSTATUS
# define WEXITSTATUS(x)	(((*(int *)&(x)) >> 8) & 0xFF)
#endif
#include <time.h>
#include <sys/param.h>
#include <pwd.h>
#include <grp.h>
/* Include <gno/gno.h> for GNO job control calls.  Define __KERN_STATUS to
 * avoid a couple definitions using ORCA/C's nonstandard inline directive.
 */
#ifdef __GNO__
# define __USE_DYNAMIC_GSSTRING__
# define __KERN_STATUS
# include <gno/gno.h>
#endif
#if ENABLE_FEATURE_SHADOWPASSWDS
# if !ENABLE_USE_BB_SHADOW
/* If using busybox's shadow implementation, do not include the shadow.h
 * header as the toolchain may not provide it at all.
 */
#  include <shadow.h>
# endif
#endif
#if defined(ANDROID) || defined(__ANDROID__)
# define endpwent() ((void)0)
# define endgrent() ((void)0)
#endif
/* Don't do this here:
 * #include <sys/sysinfo.h>
 * Some linux/ includes pull in conflicting definition
 * of struct sysinfo (only in some toolchanins), which breaks build.
 * Include sys/sysinfo.h only in those files which need it.
 */
#if ENABLE_SELINUX
# include <selinux/selinux.h>
# include <selinux/context.h>
# include <selinux/flask.h>
# include <selinux/av_permissions.h>
#endif
#if ENABLE_FEATURE_UTMP
# if defined __UCLIBC__ && ( \
    (UCLIBC_VERSION >= KERNEL_VERSION(0, 9, 32) \
     && UCLIBC_VERSION < KERNEL_VERSION(0, 9, 34) \
     && defined __UCLIBC_HAS_UTMPX__ \
    ) || ( \
	 UCLIBC_VERSION >= KERNEL_VERSION(0, 9, 34) \
	) \
  )
#  include <utmpx.h>
# elif defined __UCLIBC__
#  include <utmp.h>
#  define utmpx utmp
#  define setutxent setutent
#  define endutxent endutent
#  define getutxent getutent
#  define getutxid getutid
#  define getutxline getutline
#  define pututxline pututline
#  define utmpxname utmpname
#  define updwtmpx updwtmp
#  define _PATH_UTMPX _PATH_UTMP
# else
#  include <utmpx.h>
# endif
#endif
#if ENABLE_LOCALE_SUPPORT
# include <locale.h>
#else
# define setlocale(x,y) ((void)0)
#endif
#ifdef DMALLOC
# include <dmalloc.h>
#endif
/* Just in case libc doesn't define some of these... */
#ifndef _PATH_PASSWD
#define _PATH_PASSWD  "/etc/passwd"
#endif
#ifndef _PATH_GROUP
#define _PATH_GROUP   "/etc/group"
#endif
#ifndef _PATH_SHADOW
#define _PATH_SHADOW  "/etc/shadow"
#endif
#ifndef _PATH_GSHADOW
#define _PATH_GSHADOW "/etc/gshadow"
#endif
#if defined __FreeBSD__ || defined __OpenBSD__
# include <netinet/in.h>
# include <arpa/inet.h>
#elif defined __APPLE__
# include <netinet/in.h>
#else
# include <arpa/inet.h>
# if !defined(__socklen_t_defined) && !defined(_SOCKLEN_T_DECLARED)
/* We #define socklen_t *after* includes, otherwise we get
 * typedef redefinition errors from system headers
 * (in case "is it defined already" detection above failed)
 */
#  define socklen_t bb_socklen_t
   typedef unsigned socklen_t;
# endif
#endif
#ifndef HAVE_CLEARENV
# define clearenv() do { if (environ) environ[0] = NULL; } while (0)
#endif
#ifndef HAVE_FDATASYNC
# define fdatasync fsync
#endif


/* Some libc's forget to declare these, do it ourself */

extern char **environ;
#if defined(__GLIBC__) && __GLIBC__ < 2
int vdprintf(int d, const char *format, va_list ap);
#endif
/* klogctl is in libc's klog.h, but we cheat and not #include that */
int klogctl(int type, char *b, int len);
#ifndef PATH_MAX
# define PATH_MAX 256
#endif
#ifndef BUFSIZ
# define BUFSIZ 4096
#endif


/* Busybox does not use threads, we can speed up stdio. */
#ifdef HAVE_UNLOCKED_STDIO
# undef  getc
# define getc(stream) getc_unlocked(stream)
# undef  getchar
# define getchar() getchar_unlocked()
# undef  putc
# define putc(c, stream) putc_unlocked(c, stream)
# undef  putchar
# define putchar(c) putchar_unlocked(c)
# undef  fgetc
# define fgetc(stream) getc_unlocked(stream)
# undef  fputc
# define fputc(c, stream) putc_unlocked(c, stream)
#endif
/* Above functions are required by POSIX.1-2008, below ones are extensions */
#ifdef HAVE_UNLOCKED_LINE_OPS
# undef  fgets
# define fgets(s, n, stream) fgets_unlocked(s, n, stream)
# undef  fputs
# define fputs(s, stream) fputs_unlocked(s, stream)
#endif


/* Make all declarations hidden (-fvisibility flag only affects definitions) */
/* (don't include system headers after this until corresponding pop!) */
PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN


#if ENABLE_USE_BB_PWD_GRP
# include "pwd_.h"
# include "grp_.h"
#endif
#if ENABLE_FEATURE_SHADOWPASSWDS
# if ENABLE_USE_BB_SHADOW
#  include "shadow_.h"
# endif
#endif

/* Tested to work correctly with all int types (IIRC :]) */
#define MAXINT(T) (T)( \
	((T)-1) > 0 \
	? (T)-1 \
	: (T)~((T)1 << (sizeof(T)*8-1)) \
	)

#define MININT(T) (T)( \
	((T)-1) > 0 \
	? (T)0 \
	: ((T)1 << (sizeof(T)*8-1)) \
	)

/* Large file support */
/* Note that CONFIG_LFS=y forces bbox to be built with all common ops
 * (stat, lseek etc) mapped to "largefile" variants by libc.
 * Practically it means that open() automatically has O_LARGEFILE added
 * and all filesize/file_offset parameters and struct members are "large"
 * (in today's world - signed 64bit). For full support of large files,
 * we need a few helper #defines (below) and careful use of off_t
 * instead of int/ssize_t. No lseek64(), O_LARGEFILE etc necessary */
#if ENABLE_LFS
/* CONFIG_LFS is on */
# if ULONG_MAX > 0xffffffff
/* "long" is long enough on this system */
typedef unsigned long uoff_t;
#  define XATOOFF(a) xatoul_range((a), 0, LONG_MAX)
/* usage: sz = BB_STRTOOFF(s, NULL, 10); if (errno || sz < 0) die(); */
#  define BB_STRTOOFF bb_strtoul
#  define STRTOOFF strtoul
/* usage: printf("size: %"OFF_FMT"d (%"OFF_FMT"x)\n", sz, sz); */
#  define OFF_FMT "l"
# else
/* "long" is too short, need "long long" */
typedef unsigned long long uoff_t;
#  define XATOOFF(a) xatoull_range((a), 0, LLONG_MAX)
#  define BB_STRTOOFF bb_strtoull
#  define STRTOOFF strtoull
#  define OFF_FMT "ll"
# endif
#else
/* CONFIG_LFS is off */
# if UINT_MAX == 0xffffffff
/* While sizeof(off_t) == sizeof(int), off_t is typedef'ed to long anyway.
 * gcc will throw warnings on printf("%d", off_t). Crap... */
typedef unsigned long uoff_t;
#  define XATOOFF(a) xatoi_positive(a)
#  define BB_STRTOOFF bb_strtou
#  define STRTOOFF strtol
#  define OFF_FMT "l"
# else
typedef unsigned long uoff_t;
#  define XATOOFF(a) xatoul_range((a), 0, LONG_MAX)
#  define BB_STRTOOFF bb_strtoul
#  define STRTOOFF strtol
#  define OFF_FMT "l"
# endif
#endif
/* scary. better ideas? (but do *test* them first!) */
#define OFF_T_MAX  ((off_t)~((off_t)1 << (sizeof(off_t)*8-1)))
/* Users report bionic to use 32-bit off_t even if LARGEFILE support is requested.
 * We misdetected that. Don't let it build:
 */
struct BUG_off_t_size_is_misdetected {
	char BUG_off_t_size_is_misdetected[sizeof(off_t) == sizeof(uoff_t) ? 1 : -1];
};

/* Some useful definitions */
#undef FALSE
#define FALSE   ((int) 0)
#undef TRUE
#define TRUE    ((int) 1)
#undef SKIP
#define SKIP	((int) 2)

/* Macros for min/max.  */
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/* buffer allocation schemes */
#if ENABLE_FEATURE_BUFFERS_GO_ON_STACK
#define RESERVE_CONFIG_BUFFER(buffer,len)  char buffer[len]
#define RESERVE_CONFIG_UBUFFER(buffer,len) unsigned char buffer[len]
#define RELEASE_CONFIG_BUFFER(buffer)      ((void)0)
#else
#if ENABLE_FEATURE_BUFFERS_GO_IN_BSS
#define RESERVE_CONFIG_BUFFER(buffer,len)  static          char buffer[len]
#define RESERVE_CONFIG_UBUFFER(buffer,len) static unsigned char buffer[len]
#define RELEASE_CONFIG_BUFFER(buffer)      ((void)0)
#else
#define RESERVE_CONFIG_BUFFER(buffer,len)  char *buffer = xmalloc(len)
#define RESERVE_CONFIG_UBUFFER(buffer,len) unsigned char *buffer = xmalloc(len)
#define RELEASE_CONFIG_BUFFER(buffer)      free(buffer)
#endif
#endif

#if defined(__GLIBC__)
/* glibc uses __errno_location() to get a ptr to errno */
/* We can just memorize it once - no multithreading in busybox :) */
extern int *const bb_errno;
#undef errno
#define errno (*bb_errno)
#endif

unsigned long monotonic_sec(void) FAST_FUNC;

extern void chomp(char *s) FAST_FUNC;
extern void trim(char *s) FAST_FUNC;
extern char *skip_whitespace(const char *) FAST_FUNC;
extern char *skip_non_whitespace(const char *) FAST_FUNC;
extern char *skip_dev_pfx(const char *tty_name) FAST_FUNC;

extern char *strrstr(const char *haystack, const char *needle) FAST_FUNC;

//TODO: supply a pointer to char[11] buffer (avoid statics)?
extern const char *bb_mode_string(mode_t mode) FAST_FUNC;
extern int is_directory(const char *name, int followLinks) FAST_FUNC;
enum {	/* DO NOT CHANGE THESE VALUES!  cp.c, mv.c, install.c depend on them. */
	FILEUTILS_PRESERVE_STATUS = 1 << 0, /* -p */
	FILEUTILS_DEREFERENCE     = 1 << 1, /* !-d */
	FILEUTILS_RECUR           = 1 << 2, /* -R */
	FILEUTILS_FORCE           = 1 << 3, /* -f */
	FILEUTILS_INTERACTIVE     = 1 << 4, /* -i */
	FILEUTILS_MAKE_HARDLINK   = 1 << 5, /* -l */
	FILEUTILS_MAKE_SOFTLINK   = 1 << 6, /* -s */
	FILEUTILS_DEREF_SOFTLINK  = 1 << 7, /* -L */
	FILEUTILS_DEREFERENCE_L0  = 1 << 8, /* -H */
#if ENABLE_SELINUX
	FILEUTILS_PRESERVE_SECURITY_CONTEXT = 1 << 9, /* -c */
	FILEUTILS_SET_SECURITY_CONTEXT = 1 << 10,
#endif
	FILEUTILS_IGNORE_CHMOD_ERR = 1 << 11
};
#define FILEUTILS_CP_OPTSTR "pdRfilsLH" IF_SELINUX("c")
extern int remove_file(const char *path, int flags) FAST_FUNC;
/* NB: without FILEUTILS_RECUR in flags, it will basically "cat"
 * the source, not copy (unless "source" is a directory).
 * This makes "cp /dev/null file" and "install /dev/null file" (!!!)
 * work coreutils-compatibly. */
extern int copy_file(const char *source, const char *dest, int flags) FAST_FUNC;

enum {
	ACTION_RECURSE        = (1 << 0),
	ACTION_FOLLOWLINKS    = (1 << 1),
	ACTION_FOLLOWLINKS_L0 = (1 << 2),
	ACTION_DEPTHFIRST     = (1 << 3),
	/*ACTION_REVERSE      = (1 << 4), - unused */
	ACTION_QUIET          = (1 << 5),
	ACTION_DANGLING_OK    = (1 << 6)
};
typedef uint8_t recurse_flags_t;
extern int recursive_action(const char *fileName, unsigned flags,
	int FAST_FUNC (*fileAction)(const char *fileName, struct stat* statbuf, void* userData, int depth),
	int FAST_FUNC (*dirAction)(const char *fileName, struct stat* statbuf, void* userData, int depth),
	void* userData, unsigned depth) FAST_FUNC;
extern int device_open(const char *device, int mode) FAST_FUNC;
enum { GETPTY_BUFSIZE = 16 }; /* more than enough for "/dev/ttyXXX" */
extern int xgetpty(char *line) FAST_FUNC;
extern int get_console_fd_or_die(void) FAST_FUNC;
extern void console_make_active(int fd, const int vt_num) FAST_FUNC;
extern char *find_block_device(const char *path) FAST_FUNC;
/* bb_copyfd_XX print read/write errors and return -1 if they occur */
extern off_t bb_copyfd_eof(int fd1, int fd2) FAST_FUNC;
extern off_t bb_copyfd_size(int fd1, int fd2, off_t size) FAST_FUNC;
extern void bb_copyfd_exact_size(int fd1, int fd2, off_t size) FAST_FUNC;
/* "short" copy can be detected by return value < size */
/* this helper yells "short read!" if param is not -1 */
extern void complain_copyfd_and_die(off_t sz) NORETURN FAST_FUNC;

extern char bb_process_escape_sequence(const char **ptr) FAST_FUNC;
char* strcpy_and_process_escape_sequences(char *dst, const char *src) FAST_FUNC;
/* xxxx_strip version can modify its parameter:
 * "/"        -> "/"
 * "abc"      -> "abc"
 * "abc/def"  -> "def"
 * "abc/def/" -> "def" !!
 */
char *bb_get_last_path_component_strip(char *path) FAST_FUNC;
/* "abc/def/" -> "" and it never modifies 'path' */
char *bb_get_last_path_component_nostrip(const char *path) FAST_FUNC;
/* Simpler version: does not special case "/" string */
const char *bb_basename(const char *name) FAST_FUNC;
/* NB: can violate const-ness (similarly to strchr) */
char *last_char_is(const char *s, int c) FAST_FUNC;
const char* endofname(const char *name) FAST_FUNC;
char *is_prefixed_with(const char *string, const char *key) FAST_FUNC;
char *is_suffixed_with(const char *string, const char *key) FAST_FUNC;

int ndelay_on(int fd) FAST_FUNC;
int ndelay_off(int fd) FAST_FUNC;
void close_on_exec_on(int fd) FAST_FUNC;
void xdup2(int, int) FAST_FUNC;
void xmove_fd(int, int) FAST_FUNC;

#ifdef __GNO__
struct cloexec_ent {
	pid_t pid;
	uint32_t cloexec_mask;
};

struct cloexec_ent *get_cloexec_ent(pid_t pid);
int new_cloexec_ent(uint32_t initial_mask);
void close_cloexec_fds(void);
void close_on_exec_on(int fd);
void close_on_exec_off(int fd);
int close_wrapper(int fd);
int fclose_wrapper(FILE *stream);
int dup2_wrapper(int fd, int fd2);
# define close(fd) close_wrapper(fd)
# define fclose(stream) fclose_wrapper(stream)
# define dup2(fd, fd2) dup2_wrapper(fd, fd2)
#endif

DIR *xopendir(const char *path) FAST_FUNC;
DIR *warn_opendir(const char *path) FAST_FUNC;

char *xmalloc_realpath(const char *path) FAST_FUNC RETURNS_MALLOC;
char *xmalloc_readlink(const char *path) FAST_FUNC RETURNS_MALLOC;
char *xmalloc_readlink_or_warn(const char *path) FAST_FUNC RETURNS_MALLOC;
/* !RETURNS_MALLOC: it's a realloc-like function */
char *xrealloc_getcwd_or_warn(char *cwd) FAST_FUNC;

char *xmalloc_follow_symlinks(const char *path) FAST_FUNC RETURNS_MALLOC;


void bb_signals(long int sigs, sig_t f) FAST_FUNC;
/* syscalls like read() will be interrupted with EINTR: */
void signal_no_SA_RESTART_empty_mask(int sig, void (*handler)(int)) FAST_FUNC;
/* syscalls like read() won't be interrupted (though select/poll will be): */
void signal_SA_RESTART_empty_mask(int sig, void (*handler)(int)) FAST_FUNC;
void wait_for_any_sig(void) FAST_FUNC;
void kill_myself_with_sig(int sig) NORETURN FAST_FUNC;
void sig_block(int sig) FAST_FUNC;
void sig_unblock(int sig) FAST_FUNC;
/* Will do sigaction(signum, act, NULL): */
int sigaction_set(int sig, const struct sigaction *act) FAST_FUNC;
/* SIG_BLOCK/SIG_UNBLOCK all signals: */
int sigprocmask_allsigs(int how) FAST_FUNC;
/* Standard handler which just records signo */
extern smallint bb_got_signal;
void record_signo(int signo); /* not FAST_FUNC! */


void xsetgid(gid_t gid) FAST_FUNC;
void xsetuid(uid_t uid) FAST_FUNC;
void xsetegid(gid_t egid) FAST_FUNC;
void xseteuid(uid_t euid) FAST_FUNC;
void xchdir(const char *path) FAST_FUNC;
void xchroot(const char *path) FAST_FUNC;
void xsetenv(const char *key, const char *value) FAST_FUNC;
void bb_unsetenv(const char *key) FAST_FUNC;
void bb_unsetenv_and_free(char *key) FAST_FUNC;
void xunlink(const char *pathname) FAST_FUNC;
void xstat(const char *pathname, struct stat *buf) FAST_FUNC;
void xfstat(int fd, struct stat *buf, const char *errmsg) FAST_FUNC;
int open3_or_warn(const char *pathname, int flags, int mode) FAST_FUNC;
int open_or_warn(const char *pathname, int flags) FAST_FUNC;
int xopen3(const char *pathname, int flags, int mode) FAST_FUNC;
int xopen(const char *pathname, int flags) FAST_FUNC;
int xopen_nonblocking(const char *pathname) FAST_FUNC;
int xopen_as_uid_gid(const char *pathname, int flags, uid_t u, gid_t g) FAST_FUNC;
int open_or_warn_stdin(const char *pathname) FAST_FUNC;
int xopen_stdin(const char *pathname) FAST_FUNC;
void xrename(const char *oldpath, const char *newpath) FAST_FUNC;
int rename_or_warn(const char *oldpath, const char *newpath) FAST_FUNC;
off_t xlseek(int fd, off_t offset, int whence) FAST_FUNC;
int xmkstemp(char *template) FAST_FUNC;
off_t fdlength(int fd) FAST_FUNC;

int tc_set_to_my_pgrp(int fd);

#ifdef __GNO__
void unsetenv_wrapper(const char *var);
int putenv_wrapper(char *string);
# define unsetenv unsetenv_wrapper
# define putenv putenv_wrapper
#endif

uoff_t FAST_FUNC get_volume_size_in_bytes(int fd,
		const char *override,
		unsigned override_units,
		int extend);

void xpipe(int filedes[2]) FAST_FUNC;
/* In this form code with pipes is much more readable */
struct fd_pair { int rd; int wr; };
#define piped_pair(pair)  pipe(&((pair).rd))
#define xpiped_pair(pair) xpipe(&((pair).rd))


void parse_datestr(const char *date_str, struct tm *ptm) FAST_FUNC;
time_t validate_tm_time(const char *date_str, struct tm *ptm) FAST_FUNC;
char *strftime_HHMMSS(char *buf, unsigned len, time_t *tp) FAST_FUNC;
char *strftime_YYYYMMDDHHMMSS(char *buf, unsigned len, time_t *tp) FAST_FUNC;

int xsocket(int domain, int type, int protocol) FAST_FUNC;
void xbind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen) FAST_FUNC;
void xlisten(int s, int backlog) FAST_FUNC;
void xconnect(int s, const struct sockaddr *s_addr, socklen_t addrlen) FAST_FUNC;
ssize_t xsendto(int s, const void *buf, size_t len, const struct sockaddr *to,
				socklen_t tolen) FAST_FUNC;

int setsockopt_int(int fd, int level, int optname, int optval) FAST_FUNC;
int setsockopt_1(int fd, int level, int optname) FAST_FUNC;
int setsockopt_SOL_SOCKET_int(int fd, int optname, int optval) FAST_FUNC;
int setsockopt_SOL_SOCKET_1(int fd, int optname) FAST_FUNC;
/* SO_REUSEADDR allows a server to rebind to an address that is already
 * "in use" by old connections to e.g. previous server instance which is
 * killed or crashed. Without it bind will fail until all such connections
 * time out. Linux does not allow multiple live binds on same ip:port
 * regardless of SO_REUSEADDR (unlike some other flavors of Unix).
 * Turn it on before you call bind(). */
void setsockopt_reuseaddr(int fd) FAST_FUNC; /* On Linux this never fails. */
int setsockopt_keepalive(int fd) FAST_FUNC;
int setsockopt_broadcast(int fd) FAST_FUNC;
int setsockopt_bindtodevice(int fd, const char *iface) FAST_FUNC;
/* NB: returns port in host byte order */
unsigned bb_lookup_port(const char *port, const char *protocol, unsigned default_port) FAST_FUNC;
typedef struct len_and_sockaddr {
	socklen_t len;
	union {
		struct sockaddr sa;
#if ENABLE_FEATURE_IPV6
		struct sockaddr_in6 sin6;
#endif
	} u;
} len_and_sockaddr;

/* Create stream socket, and allocate suitable lsa.
 * (lsa of correct size and lsa->sa.sa_family (AF_INET/AF_INET6))
 * af == AF_UNSPEC will result in trying to create IPv6 socket,
 * and if kernel doesn't support it, fall back to IPv4.
 * This is useful if you plan to bind to resulting local lsa.
 */
int xsocket_type(len_and_sockaddr **lsap, int af, int sock_type) FAST_FUNC;
int xsocket_stream(len_and_sockaddr **lsap) FAST_FUNC;
/* Create server socket bound to bindaddr:port. bindaddr can be NULL,
 * numeric IP ("N.N.N.N") or numeric IPv6 address,
 * and can have ":PORT" suffix (for IPv6 use "[X:X:...:X]:PORT").
 * Only if there is no suffix, port argument is used */
/* NB: these set SO_REUSEADDR before bind */
int create_and_bind_stream_or_die(const char *bindaddr, int port) FAST_FUNC;
int create_and_bind_dgram_or_die(const char *bindaddr, int port) FAST_FUNC;
/* Create client TCP socket connected to peer:port. Peer cannot be NULL.
 * Peer can be numeric IP ("N.N.N.N"), numeric IPv6 address or hostname,
 * and can have ":PORT" suffix (for IPv6 use "[X:X:...:X]:PORT").
 * If there is no suffix, port argument is used */
int create_and_connect_stream_or_die(const char *peer, int port) FAST_FUNC;
/* Connect to peer identified by lsa */
int xconnect_stream(const len_and_sockaddr *lsa) FAST_FUNC;
/* Get local address of bound or accepted socket */
len_and_sockaddr *get_sock_lsa(int fd) FAST_FUNC RETURNS_MALLOC;
/* Get remote address of connected or accepted socket */
len_and_sockaddr *get_peer_lsa(int fd) FAST_FUNC RETURNS_MALLOC;
/* Return malloc'ed len_and_sockaddr with socket address of host:port
 * Currently will return IPv4 or IPv6 sockaddrs only
 * (depending on host), but in theory nothing prevents e.g.
 * UNIX socket address being returned, IPX sockaddr etc...
 * On error does bb_error_msg and returns NULL */
len_and_sockaddr* host2sockaddr(const char *host, int port) FAST_FUNC RETURNS_MALLOC;
/* Version which dies on error */
len_and_sockaddr* xhost2sockaddr(const char *host, int port) FAST_FUNC RETURNS_MALLOC;
len_and_sockaddr* xdotted2sockaddr(const char *host, int port) FAST_FUNC RETURNS_MALLOC;
/* Same, useful if you want to force family (e.g. IPv6) */
#if !ENABLE_FEATURE_IPV6
#define host_and_af2sockaddr(host, port, af) host2sockaddr((host), (port))
#define xhost_and_af2sockaddr(host, port, af) xhost2sockaddr((host), (port))
#else
len_and_sockaddr* host_and_af2sockaddr(const char *host, int port, sa_family_t af) FAST_FUNC RETURNS_MALLOC;
len_and_sockaddr* xhost_and_af2sockaddr(const char *host, int port, sa_family_t af) FAST_FUNC RETURNS_MALLOC;
#endif
/* Assign sin[6]_port member if the socket is an AF_INET[6] one,
 * otherwise no-op. Useful for ftp.
 * NB: does NOT do htons() internally, just direct assignment. */
void set_nport(struct sockaddr *sa, unsigned port) FAST_FUNC;
/* Retrieve sin[6]_port or return -1 for non-INET[6] lsa's */
int get_nport(const struct sockaddr *sa) FAST_FUNC;
/* Reverse DNS. Returns NULL on failure. */
char* xmalloc_sockaddr2host(const struct sockaddr *sa) FAST_FUNC RETURNS_MALLOC;
/* This one doesn't append :PORTNUM */
char* xmalloc_sockaddr2host_noport(const struct sockaddr *sa) FAST_FUNC RETURNS_MALLOC;
/* This one also doesn't fall back to dotted IP (returns NULL) */
char* xmalloc_sockaddr2hostonly_noport(const struct sockaddr *sa) FAST_FUNC RETURNS_MALLOC;
/* inet_[ap]ton on steroids */
char* xmalloc_sockaddr2dotted(const struct sockaddr *sa) FAST_FUNC RETURNS_MALLOC;
char* xmalloc_sockaddr2dotted_noport(const struct sockaddr *sa) FAST_FUNC RETURNS_MALLOC;
// "old" (ipv4 only) API
// users: traceroute.c hostname.c - use _list_ of all IPs
struct hostent *xgethostbyname(const char *name) FAST_FUNC;
// Also mount.c and inetd.c are using gethostbyname(),
// + inet_common.c has additional IPv4-only stuff


void socket_want_pktinfo(int fd) FAST_FUNC;
ssize_t send_to_from(int fd, void *buf, size_t len, int flags,
		const struct sockaddr *to,
		const struct sockaddr *from,
		socklen_t tolen) FAST_FUNC;
ssize_t recv_from_to(int fd, void *buf, size_t len, int flags,
		struct sockaddr *from,
		struct sockaddr *to,
		socklen_t sa_size) FAST_FUNC;

uint16_t inet_cksum(uint16_t *addr, int len) FAST_FUNC;

char *xstrdup(const char *s) FAST_FUNC RETURNS_MALLOC;
char *xstrndup(const char *s, int n) FAST_FUNC RETURNS_MALLOC;
void *xmemdup(const void *s, int n) FAST_FUNC RETURNS_MALLOC;
void overlapping_strcpy(char *dst, const char *src) FAST_FUNC;
char *safe_strncpy(char *dst, const char *src, size_t size) FAST_FUNC;
char *strncpy_IFNAMSIZ(char *dst, const char *src) FAST_FUNC;
unsigned count_strstr(const char *str, const char *sub) FAST_FUNC;
char *xmalloc_substitute_string(const char *src, int count, const char *sub, const char *repl) FAST_FUNC;
/* Guaranteed to NOT be a macro (smallest code). Saves nearly 2k on uclibc.
 * But potentially slow, don't use in one-billion-times loops */
int bb_putchar(int ch) FAST_FUNC;
void bb_putchar_binary(int ch) FAST_FUNC;
/* Note: does not use stdio, writes to fd 2 directly */
int bb_putchar_stderr(char ch) FAST_FUNC;
char *xasprintf(const char *format, ...) __attribute__ ((format(printf, 1, 2))) FAST_FUNC RETURNS_MALLOC;
char *auto_string(char *str) FAST_FUNC;
// gcc-4.1.1 still isn't good enough at optimizing it
// (+200 bytes compared to macro)
//static ALWAYS_INLINE
//int LONE_DASH(const char *s) { return s[0] == '-' && !s[1]; }
//static ALWAYS_INLINE
//int NOT_LONE_DASH(const char *s) { return s[0] != '-' || s[1]; }
#define LONE_DASH(s)     ((s)[0] == '-' && !(s)[1])
#define NOT_LONE_DASH(s) ((s)[0] != '-' || (s)[1])
#define LONE_CHAR(s,c)     ((s)[0] == (c) && !(s)[1])
#define NOT_LONE_CHAR(s,c) ((s)[0] != (c) || (s)[1])
#define DOT_OR_DOTDOT(s) ((s)[0] == '.' && (!(s)[1] || ((s)[1] == '.' && !(s)[2])))

typedef struct uni_stat_t {
	unsigned byte_count;
	unsigned unicode_count;
	unsigned unicode_width;
} uni_stat_t;
/* Returns a string with unprintable chars replaced by '?' or
 * SUBST_WCHAR. This function is unicode-aware. */
const char* FAST_FUNC printable_string(uni_stat_t *stats, const char *str);
/* Prints unprintable char ch as ^C or M-c to file
 * (M-c is used only if ch is ORed with PRINTABLE_META),
 * else it is printed as-is (except for ch = 0x9b) */
enum { PRINTABLE_META = 0x100 };
void fputc_printable(int ch, FILE *file) FAST_FUNC;
/* Return a string that is the printable representation of character ch.
 * Buffer must hold at least four characters. */
enum {
	VISIBLE_ENDLINE   = 1 << 0,
	VISIBLE_SHOW_TABS = 1 << 1
};
void visible(unsigned ch, char *buf, int flags) FAST_FUNC;

/* dmalloc will redefine these to it's own implementation. It is safe
 * to have the prototypes here unconditionally.  */
void *malloc_or_warn(size_t size) FAST_FUNC RETURNS_MALLOC;
void *xmalloc(size_t size) FAST_FUNC RETURNS_MALLOC;
void *xzalloc(size_t size) FAST_FUNC RETURNS_MALLOC;
void *xrealloc(void *old, size_t size) FAST_FUNC;
/* After v = xrealloc_vector(v, SHIFT, idx) it's ok to use
 * at least v[idx] and v[idx+1], for all idx values.
 * SHIFT specifies how many new elements are added (1:2, 2:4, ..., 8:256...)
 * when all elements are used up. New elements are zeroed out.
 * xrealloc_vector(v, SHIFT, idx) *MUST* be called with consecutive IDXs -
 * skipping an index is a bad bug - it may miss a realloc!
 */
#define xrealloc_vector(vector, shift, idx) \
	xrealloc_vector_helper((vector), (sizeof((vector)[0]) << 8) + (shift), (idx))
void* xrealloc_vector_helper(void *vector, unsigned sizeof_and_shift, int idx) FAST_FUNC;


extern ssize_t safe_read(int fd, void *buf, size_t count) FAST_FUNC;
extern ssize_t nonblock_immune_read(int fd, void *buf, size_t count) FAST_FUNC;
// NB: will return short read on error, not -1,
// if some data was read before error occurred
extern ssize_t full_read(int fd, void *buf, size_t count) FAST_FUNC;
extern void xread(int fd, void *buf, size_t count) FAST_FUNC;
extern unsigned char xread_char(int fd) FAST_FUNC;
extern ssize_t read_close(int fd, void *buf, size_t maxsz) FAST_FUNC;
extern ssize_t open_read_close(const char *filename, void *buf, size_t maxsz) FAST_FUNC;
// Reads one line a-la fgets (but doesn't save terminating '\n').
// Reads byte-by-byte. Useful when it is important to not read ahead.
// Bytes are appended to pfx (which must be malloced, or NULL).
extern char *xmalloc_reads(int fd, size_t *maxsz_p) FAST_FUNC;
/* Reads block up to *maxsz_p (default: INT_MAX - 4095) */
extern void *xmalloc_read(int fd, size_t *maxsz_p) FAST_FUNC RETURNS_MALLOC;
/* Returns NULL if file can't be opened (default max size: INT_MAX - 4095) */
extern void *xmalloc_open_read_close(const char *filename, size_t *maxsz_p) FAST_FUNC RETURNS_MALLOC;
/* Never returns NULL */
extern void *xmalloc_xopen_read_close(const char *filename, size_t *maxsz_p) FAST_FUNC RETURNS_MALLOC;

#if defined(ARG_MAX) && (ARG_MAX >= 60*1024 || !defined(_SC_ARG_MAX))
/* Use _constant_ maximum if: defined && (big enough || no variable one exists) */
# define bb_arg_max() ((unsigned)ARG_MAX)
#elif defined(_SC_ARG_MAX)
/* Else use variable one (a bit more expensive) */
unsigned bb_arg_max(void) FAST_FUNC;
#else
/* If all else fails */
# define bb_arg_max() ((unsigned)(32 * 1024))
#endif
unsigned bb_clk_tck(void) FAST_FUNC;

#define SEAMLESS_COMPRESSION (0 \
 || ENABLE_FEATURE_SEAMLESS_XZ \
 || ENABLE_FEATURE_SEAMLESS_LZMA \
 || ENABLE_FEATURE_SEAMLESS_BZ2 \
 || ENABLE_FEATURE_SEAMLESS_GZ \
 || ENABLE_FEATURE_SEAMLESS_Z)

extern ssize_t safe_write(int fd, const void *buf, size_t count) FAST_FUNC;
// NB: will return short write on error, not -1,
// if some data was written before error occurred
extern ssize_t full_write(int fd, const void *buf, size_t count) FAST_FUNC;
extern void xwrite(int fd, const void *buf, size_t count) FAST_FUNC;
extern void xwrite_str(int fd, const char *str) FAST_FUNC;
extern ssize_t full_write1_str(const char *str) FAST_FUNC;
extern ssize_t full_write2_str(const char *str) FAST_FUNC;
extern void xopen_xwrite_close(const char* file, const char *str) FAST_FUNC;

/* Close fd, but check for failures (some types of write errors) */
extern void xclose(int fd) FAST_FUNC;

/* Reads and prints to stdout till eof, then closes FILE. Exits on error: */
extern void xprint_and_close_file(FILE *file) FAST_FUNC;

/* Reads a line from a text file, up to a newline or NUL byte, inclusive.
 * Returns malloc'ed char*. If end is NULL '\n' isn't considered
 * end of line. If end isn't NULL, length of the chunk is stored in it.
 * Returns NULL if EOF/error.
 */
extern char *bb_get_chunk_from_file(FILE *file, int *end) FAST_FUNC;
/* Reads up to (and including) TERMINATING_STRING: */
extern char *xmalloc_fgets_str(FILE *file, const char *terminating_string) FAST_FUNC RETURNS_MALLOC;
/* Same, with limited max size, and returns the length (excluding NUL): */
extern char *xmalloc_fgets_str_len(FILE *file, const char *terminating_string, size_t *maxsz_p) FAST_FUNC RETURNS_MALLOC;
/* Chops off TERMINATING_STRING from the end: */
extern char *xmalloc_fgetline_str(FILE *file, const char *terminating_string) FAST_FUNC RETURNS_MALLOC;
/* Reads up to (and including) "\n" or NUL byte: */
extern char *xmalloc_fgets(FILE *file) FAST_FUNC RETURNS_MALLOC;
/* Chops off '\n' from the end, unlike fgets: */
extern char *xmalloc_fgetline(FILE *file) FAST_FUNC RETURNS_MALLOC;
/* Same, but doesn't try to conserve space (may have some slack after the end) */
/* extern char *xmalloc_fgetline_fast(FILE *file) FAST_FUNC RETURNS_MALLOC; */

void die_if_ferror(FILE *file, const char *msg) FAST_FUNC;
void die_if_ferror_stdout(void) FAST_FUNC;
int fflush_all(void) FAST_FUNC;
void fflush_stdout_and_exit(int retval) NORETURN FAST_FUNC;
int fclose_if_not_stdin(FILE *file) FAST_FUNC;
FILE* xfopen(const char *filename, const char *mode) FAST_FUNC;
/* Prints warning to stderr and returns NULL on failure: */
FILE* fopen_or_warn(const char *filename, const char *mode) FAST_FUNC;
/* "Opens" stdin if filename is special, else just opens file: */
FILE* xfopen_stdin(const char *filename) FAST_FUNC;
FILE* fopen_or_warn_stdin(const char *filename) FAST_FUNC;
FILE* fopen_for_read(const char *path) FAST_FUNC;
FILE* xfopen_for_read(const char *path) FAST_FUNC;
FILE* fopen_for_write(const char *path) FAST_FUNC;
FILE* xfopen_for_write(const char *path) FAST_FUNC;
FILE* xfdopen_for_read(int fd) FAST_FUNC;
FILE* xfdopen_for_write(int fd) FAST_FUNC;

int bb_pstrcmp(const void *a, const void *b) /* not FAST_FUNC! */;
void qsort_string_vector(char **sv, unsigned count) FAST_FUNC;

/* Wrapper which restarts poll on EINTR or ENOMEM.
 * On other errors complains [perror("poll")] and returns.
 * Warning! May take (much) longer than timeout_ms to return!
 * If this is a problem, use bare poll and open-code EINTR/ENOMEM handling */
int safe_poll(struct pollfd *ufds, nfds_t nfds, int timeout_ms) FAST_FUNC;

char *safe_gethostname(void) FAST_FUNC;

/* Convert each alpha char in str to lower-case */
char* str_tolower(char *str) FAST_FUNC;

char *utoa(unsigned n) FAST_FUNC;
char *itoa(int n) FAST_FUNC;
/* Returns a pointer past the formatted number, does NOT null-terminate */
char *utoa_to_buf(unsigned n, char *buf, unsigned buflen) FAST_FUNC;
char *itoa_to_buf(int n, char *buf, unsigned buflen) FAST_FUNC;
/* If block_size == 0, display size without fractional part,
 * else display (size * block_size) with one decimal digit.
 * If display_unit == 0, show value no bigger than 1024 with suffix (K,M,G...),
 * else divide by display_unit and do not use suffix. */
#define HUMAN_READABLE_MAX_WIDTH      7  /* "1024.0G" */
#define HUMAN_READABLE_MAX_WIDTH_STR "7"
/* Put a string of hex bytes ("1b2e66fe"...), return advanced pointer */
char *bin2hex(char *dst, const char *src, int count) FAST_FUNC;
/* Reverse */
char* hex2bin(char *dst, const char *src, int count) FAST_FUNC;

/* Generate a UUID */
void generate_uuid(uint8_t *buf) FAST_FUNC;

/* Last element is marked by mult == 0 */
struct suffix_mult {
	char suffix[4];
	unsigned mult;
};
extern const struct suffix_mult bkm_suffixes[];
#define km_suffixes (bkm_suffixes + 1)
extern const struct suffix_mult cwbkMG_suffixes[];
#define kMG_suffixes (cwbkMG_suffixes + 3)

#include "xatonum.h"
/* Specialized: */

/* Using xatoi() instead of naive atoi() is not always convenient -
 * in many places people want *non-negative* values, but store them
 * in signed int. Therefore we need this one:
 * dies if input is not in [0, INT_MAX] range. Also will reject '-0' etc.
 * It should really be named xatoi_nonnegative (since it allows 0),
 * but that would be too long.
 */
int xatoi_positive(const char *numstr) FAST_FUNC;

/* Useful for reading port numbers */
uint16_t xatou16(const char *numstr) FAST_FUNC;


/* These parse entries in /etc/passwd and /etc/group.  This is desirable
 * for BusyBox since we want to avoid using the glibc NSS stuff, which
 * increases target size and is often not needed on embedded systems.  */
long xuname2uid(const char *name) FAST_FUNC;
long xgroup2gid(const char *name) FAST_FUNC;
/* wrapper: allows string to contain numeric uid or gid */
unsigned long get_ug_id(const char *s, long FAST_FUNC (*xname2id)(const char *)) FAST_FUNC;
/* from chpst. Does not die, returns 0 on failure */
struct bb_uidgid_t {
	uid_t uid;
	gid_t gid;
};
/* always sets uid and gid */
int get_uidgid(struct bb_uidgid_t*, const char*, int numeric_ok) FAST_FUNC;
/* always sets uid and gid, allows numeric; exits on failure */
void xget_uidgid(struct bb_uidgid_t*, const char*) FAST_FUNC;
/* chown-like handling of "user[:[group]" */
void parse_chown_usergroup_or_die(struct bb_uidgid_t *u, char *user_group) FAST_FUNC;
struct passwd* xgetpwnam(const char *name) FAST_FUNC;
struct group* xgetgrnam(const char *name) FAST_FUNC;
struct passwd* xgetpwuid(uid_t uid) FAST_FUNC;
struct group* xgetgrgid(gid_t gid) FAST_FUNC;
char* xuid2uname(uid_t uid) FAST_FUNC;
char* xgid2group(gid_t gid) FAST_FUNC;
char* uid2uname(uid_t uid) FAST_FUNC;
char* gid2group(gid_t gid) FAST_FUNC;
char* uid2uname_utoa(uid_t uid) FAST_FUNC;
char* gid2group_utoa(gid_t gid) FAST_FUNC;
/* versions which cache results (useful for ps, ls etc) */
const char* get_cached_username(uid_t uid) FAST_FUNC;
const char* get_cached_groupname(gid_t gid) FAST_FUNC;
void clear_username_cache(void) FAST_FUNC;
/* internally usernames are saved in fixed-sized char[] buffers */
enum { USERNAME_MAX_SIZE = 32 - sizeof(uid_t) };
#if ENABLE_FEATURE_CHECK_NAMES
void die_if_bad_username(const char* name) FAST_FUNC;
#else
#define die_if_bad_username(name) ((void)(name))
#endif

int ingroup(uid_t u, gid_t g);

#if ENABLE_FEATURE_UTMP
void FAST_FUNC write_new_utmp(pid_t pid, int new_type, const char *tty_name, const char *username, const char *hostname);
void FAST_FUNC update_utmp(pid_t pid, int new_type, const char *tty_name, const char *username, const char *hostname);
void FAST_FUNC update_utmp_DEAD_PROCESS(pid_t pid);
#else
# define write_new_utmp(pid, new_type, tty_name, username, hostname) ((void)0)
# define update_utmp(pid, new_type, tty_name, username, hostname) ((void)0)
# define update_utmp_DEAD_PROCESS(pid) ((void)0)
#endif


int file_is_executable(const char *name) FAST_FUNC;
char *find_executable(const char *filename, char **PATHp) FAST_FUNC;
int executable_exists(const char *filename) FAST_FUNC;

/* xvfork() can't be a _function_, return after vfork mangles stack
 * in the parent. It must be a macro. */
#define xvfork() \
({ \
	pid_t bb__xvfork_pid = vfork(); \
	if (bb__xvfork_pid < 0) \
		bb_perror_msg_and_die("vfork"); \
	bb__xvfork_pid; \
})
#if BB_MMU
pid_t xfork(void) FAST_FUNC;
#endif

/* NOMMU friendy fork+exec: */
pid_t spawn(char **argv) FAST_FUNC;
pid_t xspawn(char **argv) FAST_FUNC;

pid_t safe_waitpid(pid_t pid, int *wstat, int options) FAST_FUNC;
/* wait4pid: unlike waitpid, waits ONLY for one process.
 * Returns sig + 0x180 if child is killed by signal.
 * It's safe to pass negative 'pids' from failed [v]fork -
 * wait4pid will return -1 (and will not clobber [v]fork's errno).
 * IOW: rc = wait4pid(spawn(argv));
 *      if (rc < 0) bb_perror_msg("%s", argv[0]);
 *      if (rc > 0) bb_error_msg("exit code: %d", rc & 0xff);
 */
int wait4pid(pid_t pid) FAST_FUNC;
/* Same as wait4pid(spawn(argv)), but with NOFORK/NOEXEC if configured: */
int spawn_and_wait(char **argv) FAST_FUNC;
/* Does NOT check that applet is NOFORK, just blindly runs it */
int run_nofork_applet(int applet_no, char **argv) FAST_FUNC;

/* Helpers for daemonization.
 *
 * bb_daemonize(flags) = daemonize, does not compile on NOMMU
 *
 * bb_daemonize_or_rexec(flags, argv) = daemonizes on MMU (and ignores argv),
 *      rexec's itself on NOMMU with argv passed as command line.
 * Thus bb_daemonize_or_rexec may cause your <applet>_main() to be re-executed
 * from the start. (It will detect it and not reexec again second time).
 * You have to audit carefully that you don't do something twice as a result
 * (opening files/sockets, parsing config files etc...)!
 *
 * Both of the above will redirect fd 0,1,2 to /dev/null and drop ctty
 * (will do setsid()).
 *
 * fork_or_rexec(argv) = bare-bones fork on MMU,
 *      "vfork + re-exec ourself" on NOMMU. No fd redirection, no setsid().
 *      On MMU ignores argv.
 *
 * Helper for network daemons in foreground mode:
 *
 * bb_sanitize_stdio() = make sure that fd 0,1,2 are opened by opening them
 * to /dev/null if they are not.
 */
enum {
	DAEMON_CHDIR_ROOT = 1,
	DAEMON_DEVNULL_STDIO = 2,
	DAEMON_CLOSE_EXTRA_FDS = 4,
	DAEMON_ONLY_SANITIZE = 8, /* internal use */
	DAEMON_DOUBLE_FORK = 16 /* double fork to avoid controlling tty */
};
#if BB_MMU
# define fork_or_rexec(argv)                xfork()
# define bb_daemonize_or_rexec(flags, argv) bb_daemonize_or_rexec(flags)
# define bb_daemonize(flags)                bb_daemonize_or_rexec(flags, bogus)
#else
  /* Note: re_exec() and fork_or_rexec() do argv[0][0] |= 0x80 on NOMMU!
   * _Parent_ needs to undo it if it doesn't want to have argv[0] mangled.
   */
  pid_t fork_or_rexec(char **argv) FAST_FUNC;
  int  BUG_fork_is_unavailable_on_nommu(void) FAST_FUNC;
  int  BUG_daemon_is_unavailable_on_nommu(void) FAST_FUNC;
  void BUG_bb_daemonize_is_unavailable_on_nommu(void) FAST_FUNC;
# define fork()          BUG_fork_is_unavailable_on_nommu()
# define xfork()         BUG_fork_is_unavailable_on_nommu()
# define daemon(a,b)     BUG_daemon_is_unavailable_on_nommu()
# define bb_daemonize(a) BUG_bb_daemonize_is_unavailable_on_nommu()
#endif
void bb_daemonize_or_rexec(int flags, char **argv) FAST_FUNC;
void bb_sanitize_stdio(void) FAST_FUNC;
/* Clear dangerous stuff, set PATH. Return 1 if was run by different user. */
int sanitize_env_if_suid(void) FAST_FUNC;


pid_t vfork_and_run(void (*fn)(void*) NORETURN, void *arg);
pid_t xvfork_and_run(void (*fn)(void*) NORETURN, void *arg);


char* single_argv(char **argv) FAST_FUNC;
extern const char *const bb_argv_dash[]; /* "-", NULL */
extern const char *opt_complementary;
#if ENABLE_LONG_OPTS || ENABLE_FEATURE_GETOPT_LONG
#define No_argument "\0"
#define Required_argument "\001"
#define Optional_argument "\002"
extern const char *applet_long_options;
#endif
extern uint32_t option_mask32;
extern uint32_t getopt32(char **argv, const char *applet_opts, ...) FAST_FUNC;


/* Having next pointer as a first member allows easy creation
 * of "llist-compatible" structs, and using llist_FOO functions
 * on them.
 */
typedef struct llist_t {
	struct llist_t *link;
	char *data;
} llist_t;
void llist_add_to(llist_t **old_head, void *data) FAST_FUNC;
void llist_add_to_end(llist_t **list_head, void *data) FAST_FUNC;
void *llist_pop(llist_t **elm) FAST_FUNC;
void llist_unlink(llist_t **head, llist_t *elm) FAST_FUNC;
void llist_free(llist_t *elm, void (*freeit)(void *data)) FAST_FUNC;
llist_t *llist_rev(llist_t *list) FAST_FUNC;
llist_t *llist_find_str(llist_t *first, const char *str) FAST_FUNC;
/* BTW, surprisingly, changing API to
 *   llist_t *llist_add_to(llist_t *old_head, void *data)
 * etc does not result in smaller code... */

enum {
	LOGMODE_NONE = 0,
	LOGMODE_STDIO = (1 << 0)
};
extern const char *msg_eol;
extern smallint logmode;
extern uint8_t xfunc_error_retval;
extern void (*die_func)(void);
extern void xfunc_die(void) NORETURN FAST_FUNC;
extern void bb_show_usage(void) NORETURN FAST_FUNC;
extern void bb_error_msg(const char *s, ...) __attribute__ ((format (printf, 1, 2))) FAST_FUNC;
extern void bb_error_msg_and_die(const char *s, ...) __attribute__ ((noreturn, format (printf, 1, 2))) FAST_FUNC;
extern void bb_perror_msg(const char *s, ...) __attribute__ ((format (printf, 1, 2))) FAST_FUNC;
extern void bb_simple_perror_msg(const char *s) FAST_FUNC;
extern void bb_perror_msg_and_die(const char *s, ...) __attribute__ ((noreturn, format (printf, 1, 2))) FAST_FUNC;
extern void bb_simple_perror_msg_and_die(const char *s) NORETURN FAST_FUNC;
extern void bb_herror_msg(const char *s, ...) __attribute__ ((format (printf, 1, 2))) FAST_FUNC;
extern void bb_herror_msg_and_die(const char *s, ...) __attribute__ ((noreturn, format (printf, 1, 2))) FAST_FUNC;
extern void bb_perror_nomsg_and_die(void) NORETURN FAST_FUNC;
extern void bb_perror_nomsg(void) FAST_FUNC;
extern void bb_info_msg(const char *s, ...) __attribute__ ((format (printf, 1, 2))) FAST_FUNC;
extern void bb_verror_msg(const char *s, va_list p, const char *strerr) FAST_FUNC;

/* We need to export XXX_main from libbusybox
 * only if we build "individual" binaries
 */
#if ENABLE_FEATURE_INDIVIDUAL
#define MAIN_EXTERNALLY_VISIBLE EXTERNALLY_VISIBLE
#else
#define MAIN_EXTERNALLY_VISIBLE
#endif


/* Applets which are useful from another applets */
int bb_cat(char** argv);
/* If shell needs them, they exist even if not enabled as applets */
int echo_main(int argc, char** argv);
int printf_main(int argc, char **argv);
int test_main(int argc, char **argv);
int kill_main(int argc, char **argv);
/* Similar, but used by chgrp, not shell */
int chown_main(int argc, char **argv);
/* Used by ftpd */
int ls_main(int argc, char **argv);
/* Don't need IF_xxx() guard for these */
int gunzip_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int bunzip2_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;

#if ENABLE_ROUTE
void bb_displayroutes(int noresolve, int netstatfmt) FAST_FUNC;
#endif


/* Networking */
/* This structure defines protocol families and their handlers. */
struct aftype {
	const char *name;
	const char *title;
	int af;
	int alen;
	char*       FAST_FUNC (*print)(unsigned char *);
	const char* FAST_FUNC (*sprint)(struct sockaddr *, int numeric);
	int         FAST_FUNC (*input)(/*int type,*/ const char *bufp, struct sockaddr *);
	void        FAST_FUNC (*herror)(char *text);
	int         FAST_FUNC (*rprint)(int options);
	int         FAST_FUNC (*rinput)(int typ, int ext, char **argv);
	/* may modify src */
	int         FAST_FUNC (*getmask)(char *src, struct sockaddr *mask, char *name);
};
/* This structure defines hardware protocols and their handlers. */
struct hwtype {
	const char *name;
	const char *title;
	int type;
	int alen;
	char* FAST_FUNC (*print)(unsigned char *);
	int   FAST_FUNC (*input)(const char *, struct sockaddr *);
	int   FAST_FUNC (*activate)(int fd);
	int suppress_null_addr;
};
int display_interfaces(char *ifname) FAST_FUNC;
int in_ether(const char *bufp, struct sockaddr *sap) FAST_FUNC;
#if ENABLE_FEATURE_HWIB
int in_ib(const char *bufp, struct sockaddr *sap) FAST_FUNC;
#else
#define in_ib(a, b) 1 /* fail */
#endif
const struct aftype *get_aftype(const char *name) FAST_FUNC;
const struct hwtype *get_hwtype(const char *name) FAST_FUNC;
const struct hwtype *get_hwntype(int type) FAST_FUNC;


#ifndef BUILD_INDIVIDUAL
extern int find_applet_by_name(const char *name) FAST_FUNC;
/* Returns only if applet is not found. */
extern void run_applet_and_exit(const char *name, char **argv) FAST_FUNC;
extern void run_applet_no_and_exit(int a, char **argv) NORETURN FAST_FUNC;
#endif

#ifdef HAVE_MNTENT_H
extern int match_fstype(const struct mntent *mt, const char *fstypes) FAST_FUNC;
extern struct mntent *find_mount_point(const char *name, int subdir_too) FAST_FUNC;
#endif
extern void erase_mtab(const char * name) FAST_FUNC;
#if ENABLE_DESKTOP
extern void bb_warn_ignoring_args(char *arg) FAST_FUNC;
#else
# define bb_warn_ignoring_args(arg) ((void)0)
#endif

extern int get_linux_version_code(void) FAST_FUNC;

extern char *query_loop(const char *device) FAST_FUNC;
extern int del_loop(const char *device) FAST_FUNC;

/* Like bb_ask below, but asks on stdin with no timeout.  */
char *bb_ask_stdin(const char * prompt) FAST_FUNC;
//TODO: pass buf pointer or return allocated buf (avoid statics)?
char *bb_ask(const int fd, int timeout, const char * prompt) FAST_FUNC;
int bb_ask_confirmation(void) FAST_FUNC;

/* Returns -1 if input is invalid. current_mode is a base for e.g. "u+rw" */
int bb_parse_mode(const char* s, unsigned cur_mode) FAST_FUNC;

/*
 * Config file parser
 */
typedef struct parser_t {
	FILE *fp;
	char *data;
	char *line, *nline;
	size_t line_alloc, nline_alloc;
	int lineno;
} parser_t;
parser_t* config_open(const char *filename) FAST_FUNC;
parser_t* config_open2(const char *filename, FILE* FAST_FUNC (*fopen_func)(const char *path)) FAST_FUNC;
/* delims[0] is a comment char (use '\0' to disable), the rest are token delimiters */
int config_read(parser_t *parser, char **tokens, unsigned flags, const char *delims) FAST_FUNC;
#define config_read(parser, tokens, max, min, str, flags) \
	config_read(parser, tokens, ((flags) | (((min) & 0xFF) << 8) | ((max) & 0xFF)), str)
void config_close(parser_t *parser) FAST_FUNC;

/* Concatenate path and filename to new allocated buffer.
 * Add "/" only as needed (no duplicate "//" are produced).
 * If path is NULL, it is assumed to be "/".
 * filename should not be NULL. */
char *concat_path_file(const char *path, const char *filename) FAST_FUNC;
/* Returns NULL on . and .. */
char *concat_subpath_file(const char *path, const char *filename) FAST_FUNC;


int bb_make_directory(char *path, long mode, int flags) FAST_FUNC;

int get_signum(const char *name) FAST_FUNC;
const char *get_signame(int number) FAST_FUNC;
void print_signames(void) FAST_FUNC;

char *bb_simplify_path(const char *path) FAST_FUNC;
/* Returns ptr to NUL */
char *bb_simplify_abs_path_inplace(char *path) FAST_FUNC;

#ifndef LOGIN_FAIL_DELAY
#define LOGIN_FAIL_DELAY 3
#endif
extern void bb_do_delay(int seconds) FAST_FUNC;
extern void change_identity(const struct passwd *pw) FAST_FUNC;
extern void run_shell(const char *shell, int loginshell, const char *command, const char **additional_args) NORETURN FAST_FUNC;

/* Returns $SHELL, getpwuid(getuid())->pw_shell, or DEFAULT_SHELL.
 * Note that getpwuid result might need xstrdup'ing
 * if there is a possibility of intervening getpwxxx() calls.
 */
const char *get_shell_name(void) FAST_FUNC;

#if ENABLE_SELINUX
extern void renew_current_security_context(void) FAST_FUNC;
extern void set_current_security_context(security_context_t sid) FAST_FUNC;
extern context_t set_security_context_component(security_context_t cur_context,
						char *user, char *role, char *type, char *range) FAST_FUNC;
extern void setfscreatecon_or_die(security_context_t scontext) FAST_FUNC;
extern void selinux_preserve_fcontext(int fdesc) FAST_FUNC;
#else
#define selinux_preserve_fcontext(fdesc) ((void)0)
#endif
extern void selinux_or_die(void) FAST_FUNC;


/* systemd support */
#define SD_LISTEN_FDS_START 3
int sd_listen_fds(void);


/* setup_environment:
 * if chdir pw->pw_dir: ok: else if to_tmp == 1: goto /tmp else: goto / or die
 * if clear_env = 1: cd(pw->pw_dir), clear environment, then set
 *   TERM=(old value)
 *   USER=pw->pw_name, LOGNAME=pw->pw_name
 *   PATH=bb_default_[root_]path
 *   HOME=pw->pw_dir
 *   SHELL=shell
 * else if change_env = 1:
 *   if not root (if pw->pw_uid != 0):
 *     USER=pw->pw_name, LOGNAME=pw->pw_name
 *   HOME=pw->pw_dir
 *   SHELL=shell
 * else does nothing
 */
#define SETUP_ENV_CHANGEENV (1 << 0)
#define SETUP_ENV_CLEARENV  (1 << 1)
#define SETUP_ENV_TO_TMP    (1 << 2)
#define SETUP_ENV_NO_CHDIR  (1 << 4)
void setup_environment(const char *shell, int flags, const struct passwd *pw) FAST_FUNC;
void nuke_str(char *str) FAST_FUNC;
int check_password(const struct passwd *pw, const char *plaintext) FAST_FUNC;
int ask_and_check_password_extended(const struct passwd *pw, int timeout, const char *prompt) FAST_FUNC;
int ask_and_check_password(const struct passwd *pw) FAST_FUNC;
/* Returns a malloced string */
#if !ENABLE_USE_BB_CRYPT
#define pw_encrypt(clear, salt, cleanup) pw_encrypt(clear, salt)
#endif
extern char *pw_encrypt(const char *clear, const char *salt, int cleanup) FAST_FUNC;
extern int obscure(const char *old, const char *newval, const struct passwd *pwdp) FAST_FUNC;
/*
 * rnd is additional random input. New one is returned.
 * Useful if you call crypt_make_salt many times in a row:
 * rnd = crypt_make_salt(buf1, 4, 0);
 * rnd = crypt_make_salt(buf2, 4, rnd);
 * rnd = crypt_make_salt(buf3, 4, rnd);
 * (otherwise we risk having same salt generated)
 */
extern int crypt_make_salt(char *p, int cnt /*, int rnd*/) FAST_FUNC;
/* "$N$" + sha_salt_16_bytes + NUL */
#define MAX_PW_SALT_LEN (3 + 16 + 1)
extern char* crypt_make_pw_salt(char p[MAX_PW_SALT_LEN], const char *algo) FAST_FUNC;


/* Returns number of lines changed, or -1 on error */
#if !(ENABLE_FEATURE_ADDUSER_TO_GROUP || ENABLE_FEATURE_DEL_USER_FROM_GROUP)
#define update_passwd(filename, username, data, member) \
	update_passwd(filename, username, data)
#endif
extern int update_passwd(const char *filename,
		const char *username,
		const char *data,
		const char *member) FAST_FUNC;

int index_in_str_array(const char *const string_array[], const char *key) FAST_FUNC;
int index_in_strings(const char *strings, const char *key) FAST_FUNC;
int index_in_substr_array(const char *const string_array[], const char *key) FAST_FUNC;
int index_in_substrings(const char *strings, const char *key) FAST_FUNC;
const char *nth_string(const char *strings, int n) FAST_FUNC;

extern void print_login_issue(const char *issue_file, const char *tty) FAST_FUNC;
extern void print_login_prompt(void) FAST_FUNC;

char *xmalloc_ttyname(int fd) FAST_FUNC RETURNS_MALLOC;
/* NB: typically you want to pass fd 0, not 1. Think 'applet | grep something' */
int get_terminal_width_height(int fd, unsigned *width, unsigned *height) FAST_FUNC;

#ifndef __GNO__
int tcsetattr_stdin_TCSANOW(const struct termios *tp) FAST_FUNC;
#endif

/* NB: "unsigned request" is crucial! "int request" will break some arches! */
int ioctl_or_perror(int fd, unsigned request, void *argp, const char *fmt,...) __attribute__ ((format (printf, 4, 5))) FAST_FUNC;
int ioctl_or_perror_and_die(int fd, unsigned request, void *argp, const char *fmt,...) __attribute__ ((format (printf, 4, 5))) FAST_FUNC;
#if ENABLE_IOCTL_HEX2STR_ERROR
int bb_ioctl_or_warn(int fd, unsigned request, void *argp, const char *ioctl_name) FAST_FUNC;
int bb_xioctl(int fd, unsigned request, void *argp, const char *ioctl_name) FAST_FUNC;
#define ioctl_or_warn(fd,request,argp) bb_ioctl_or_warn(fd,request,argp,#request)
#define xioctl(fd,request,argp)        bb_xioctl(fd,request,argp,#request)
#else
int bb_ioctl_or_warn(int fd, unsigned request, void *argp) FAST_FUNC;
int bb_xioctl(int fd, unsigned request, void *argp) FAST_FUNC;
#define ioctl_or_warn(fd,request,argp) bb_ioctl_or_warn(fd,request,argp)
#define xioctl(fd,request,argp)        bb_xioctl(fd,request,argp)
#endif

char *is_in_ino_dev_hashtable(const struct stat *statbuf) FAST_FUNC;
void add_to_ino_dev_hashtable(const struct stat *statbuf, const char *name) FAST_FUNC;
void reset_ino_dev_hashtable(void) FAST_FUNC;
#ifdef __GLIBC__
/* At least glibc has horrendously large inline for this, so wrap it */
unsigned long long bb_makedev(unsigned major, unsigned minor) FAST_FUNC;
#undef makedev
#define makedev(a,b) bb_makedev(a,b)
#endif


/* "Keycodes" that report an escape sequence.
 * We use something which fits into signed char,
 * yet doesn't represent any valid Unicode character.
 * Also, -1 is reserved for error indication and we don't use it. */
enum {
	KEYCODE_UP       =  -2,
	KEYCODE_DOWN     =  -3,
	KEYCODE_RIGHT    =  -4,
	KEYCODE_LEFT     =  -5,
	KEYCODE_HOME     =  -6,
	KEYCODE_END      =  -7,
	KEYCODE_INSERT   =  -8,
	KEYCODE_DELETE   =  -9,
	KEYCODE_PAGEUP   = -10,
	KEYCODE_PAGEDOWN = -11,
	// -12 is reserved for Alt/Ctrl/Shift-TAB
#if 0
	KEYCODE_FUN1     = -13,
	KEYCODE_FUN2     = -14,
	KEYCODE_FUN3     = -15,
	KEYCODE_FUN4     = -16,
	KEYCODE_FUN5     = -17,
	KEYCODE_FUN6     = -18,
	KEYCODE_FUN7     = -19,
	KEYCODE_FUN8     = -20,
	KEYCODE_FUN9     = -21,
	KEYCODE_FUN10    = -22,
	KEYCODE_FUN11    = -23,
	KEYCODE_FUN12    = -24,
#endif
	/* Be sure that last defined value is small enough
	 * to not interfere with Alt/Ctrl/Shift bits.
	 * So far we do not exceed -31 (0xfff..fffe1),
	 * which gives us three upper bits in LSB to play with.
	 */
	//KEYCODE_SHIFT_TAB  = (-12)         & ~0x80,
	//KEYCODE_SHIFT_...  = KEYCODE_...   & ~0x80,
	//KEYCODE_CTRL_UP    = KEYCODE_UP    & ~0x40,
	//KEYCODE_CTRL_DOWN  = KEYCODE_DOWN  & ~0x40,
	KEYCODE_CTRL_RIGHT = KEYCODE_RIGHT & ~0x40,
	KEYCODE_CTRL_LEFT  = KEYCODE_LEFT  & ~0x40,
	//KEYCODE_ALT_UP     = KEYCODE_UP    & ~0x20,
	//KEYCODE_ALT_DOWN   = KEYCODE_DOWN  & ~0x20,
	KEYCODE_ALT_RIGHT  = KEYCODE_RIGHT & ~0x20,
	KEYCODE_ALT_LEFT   = KEYCODE_LEFT  & ~0x20,

	KEYCODE_CURSOR_POS = -0x100, /* 0xfff..fff00 */
	/* How long is the longest ESC sequence we know?
	 * We want it big enough to be able to contain
	 * cursor position sequence "ESC [ 9999 ; 9999 R"
	 */
	KEYCODE_BUFFER_SIZE = 16
};
/* Note: fd may be in blocking or non-blocking mode, both make sense.
 * For one, less uses non-blocking mode.
 * Only the first read syscall inside read_key may block indefinitely
 * (unless fd is in non-blocking mode),
 * subsequent reads will time out after a few milliseconds.
 * Return of -1 means EOF or error (errno == 0 on EOF).
 * buffer[0] is used as a counter of buffered chars and must be 0
 * on first call.
 * timeout:
 * -2: do not poll for input;
 * -1: poll(-1) (i.e. block);
 * >=0: poll for TIMEOUT milliseconds, return -1/EAGAIN on timeout
 */
int32_t read_key(int fd, char *buffer, int timeout) FAST_FUNC;
void read_key_ungets(char *buffer, const char *str, unsigned len) FAST_FUNC;

/* Terminal escape sequences to process in input */
struct escape_seq {
	char *seq;
	signed char keycode;
};
extern struct escape_seq escape_seqs[];
extern int n_escape_seqs;

#if ENABLE_FEATURE_EDITING
/* It's NOT just ENABLEd or disabled. It's a number: */
# if defined CONFIG_FEATURE_EDITING_HISTORY && CONFIG_FEATURE_EDITING_HISTORY > 0
#  define MAX_HISTORY (CONFIG_FEATURE_EDITING_HISTORY + 0)
unsigned size_from_HISTFILESIZE(const char *hp) FAST_FUNC;
# else
#  define MAX_HISTORY 0
# endif
typedef struct line_input_t {
	int flags;
	const char *path_lookup;
# if MAX_HISTORY
	int cnt_history;
	int cur_history;
	int max_history; /* must never be <= 0 */
#  if ENABLE_FEATURE_EDITING_SAVEHISTORY
	/* meaning of this field depends on FEATURE_EDITING_SAVE_ON_EXIT:
	 * if !FEATURE_EDITING_SAVE_ON_EXIT: "how many lines are
	 * in on-disk history"
	 * if FEATURE_EDITING_SAVE_ON_EXIT: "how many in-memory lines are
	 * also in on-disk history (and thus need to be skipped on save)"
	 */
	unsigned cnt_history_in_file;
	const char *hist_file;
#  endif
	char *history[MAX_HISTORY + 1];
# endif
} line_input_t;
enum {
	DO_HISTORY       = 1 * (MAX_HISTORY > 0),
	TAB_COMPLETION   = 2 * ENABLE_FEATURE_TAB_COMPLETION,
	USERNAME_COMPLETION = 4 * ENABLE_FEATURE_USERNAME_COMPLETION,
	VI_MODE          = 8 * ENABLE_FEATURE_EDITING_VI,
	WITH_PATH_LOOKUP = 0x10,
	FOR_SHELL        = DO_HISTORY | TAB_COMPLETION | USERNAME_COMPLETION
};
line_input_t *new_line_input_t(int flags) FAST_FUNC;
/* So far static: void free_line_input_t(line_input_t *n) FAST_FUNC; */
/*
 * maxsize must be >= 2.
 * Returns:
 * -1 on read errors or EOF, or on bare Ctrl-D,
 * 0  on ctrl-C (the line entered is still returned in 'command'),
 * >0 length of input string, including terminating '\n'
 */
int read_line_input(line_input_t *st, const char *prompt, char *command, int maxsize, int timeout) FAST_FUNC;
void show_history(const line_input_t *st) FAST_FUNC;
# if ENABLE_FEATURE_EDITING_SAVE_ON_EXIT
void save_history(line_input_t *st);
# endif
#else
#define MAX_HISTORY 0
int read_line_input(const char* prompt, char* command, int maxsize) FAST_FUNC;
#define read_line_input(state, prompt, command, maxsize, timeout) \
	read_line_input(prompt, command, maxsize)
#endif


#ifndef COMM_LEN
# ifdef TASK_COMM_LEN
enum { COMM_LEN = TASK_COMM_LEN };
# else
/* synchronize with sizeof(task_struct.comm) in /usr/include/linux/sched.h */
enum { COMM_LEN = 16 };
# endif
#endif

struct smaprec {
	unsigned long mapped_rw;
	unsigned long mapped_ro;
	unsigned long shared_clean;
	unsigned long shared_dirty;
	unsigned long private_clean;
	unsigned long private_dirty;
	unsigned long stack;
	unsigned long smap_pss, smap_swap;
	unsigned long smap_size;
	unsigned long smap_start;
	char smap_mode[5];
	char *smap_name;
};

#if !ENABLE_PMAP
#define procps_read_smaps(pid, total, cb, data) \
	procps_read_smaps(pid, total)
#endif
int FAST_FUNC procps_read_smaps(pid_t pid, struct smaprec *total,
		void (*cb)(struct smaprec *, void *), void *data);


/* Format cmdline (up to col chars) into char buf[size] */
/* Puts [comm] if cmdline is empty (-> process is a kernel thread) */
void read_cmdline(char *buf, int size, unsigned pid, const char *comm) FAST_FUNC;
pid_t *find_pid_by_name(const char* procName) FAST_FUNC;
pid_t *pidlist_reverse(pid_t *pidList) FAST_FUNC;
int starts_with_cpu(const char *str) FAST_FUNC;
unsigned get_cpu_count(void) FAST_FUNC;


/* Use strict=1 if you process input from untrusted source:
 * it will return NULL on invalid %xx (bad hex chars)
 * and str + 1 if decoded char is / or NUL.
 * In non-strict mode, it always succeeds (returns str),
 * and also it additionally decoded '+' to space.
 */
char *percent_decode_in_place(char *str, int strict) FAST_FUNC;


uint32_t *crc32_filltable(uint32_t *tbl256, int endian) FAST_FUNC;
uint32_t crc32_block_endian1(uint32_t val, const void *buf, unsigned len, uint32_t *crc_table) FAST_FUNC;
uint32_t crc32_block_endian0(uint32_t val, const void *buf, unsigned len, uint32_t *crc_table) FAST_FUNC;

typedef struct masks_labels_t {
	const char *labels;
	const int masks[];
} masks_labels_t;
int print_flags_separated(const int *masks, const char *labels,
		int flags, const char *separator) FAST_FUNC;
int print_flags(const masks_labels_t *ml, int flags) FAST_FUNC;

typedef struct bb_progress_t {
	unsigned last_size;
	unsigned last_update_sec;
	unsigned last_change_sec;
	unsigned start_sec;
	const char *curfile;
} bb_progress_t;

#define is_bb_progress_inited(p) ((p)->curfile != NULL)
#define bb_progress_free(p) do { \
	if (ENABLE_UNICODE_SUPPORT) free((char*)((p)->curfile)); \
	(p)->curfile = NULL; \
} while (0)
void bb_progress_init(bb_progress_t *p, const char *curfile) FAST_FUNC;
void bb_progress_update(bb_progress_t *p,
			uoff_t beg_range,
			uoff_t transferred,
			uoff_t totalsize) FAST_FUNC;


/* Some older linkers don't perform string merging, we used to have common strings
 * as global arrays to do it by hand. But:
 * (1) newer linkers do it themselves,
 * (2) however, they DONT merge string constants with global arrays,
 * even if the value is the same (!). Thus global arrays actually
 * increased size a bit: for example, "/etc/passwd" string from libc
 * wasn't merged with bb_path_passwd_file[] array!
 * Therefore now we use #defines.
 */
/* "BusyBox vN.N.N (timestamp or extra_version)" */
extern const char bb_banner[] ALIGN1;
extern const char bb_msg_memory_exhausted[] ALIGN1;
extern const char bb_msg_invalid_date[] ALIGN1;
#define bb_msg_read_error "read error"
#define bb_msg_write_error "write error"
extern const char bb_msg_unknown[] ALIGN1;
extern const char bb_msg_can_not_create_raw_socket[] ALIGN1;
extern const char bb_msg_perm_denied_are_you_root[] ALIGN1;
extern const char bb_msg_you_must_be_root[] ALIGN1;
extern const char bb_msg_requires_arg[] ALIGN1;
extern const char bb_msg_invalid_arg[] ALIGN1;
extern const char bb_msg_standard_input[] ALIGN1;
extern const char bb_msg_standard_output[] ALIGN1;

/* NB: (bb_hexdigits_upcase[i] | 0x20) -> lowercase hex digit */
extern const char bb_hexdigits_upcase[] ALIGN1;

extern const char bb_path_wtmp_file[] ALIGN1;

#ifndef __GNO__
# define bb_dev_null "/dev/null"
#else
# define bb_dev_null ".null"
#endif

extern const int const_int_0;
//extern const int const_int_1;


/* Providing hard guarantee on minimum size (think of BUFSIZ == 128) */
enum { COMMON_BUFSIZE = (BUFSIZ >= 256*sizeof(void*) ? BUFSIZ+1 : 256*sizeof(void*)) };
extern char bb_common_bufsiz1[COMMON_BUFSIZE];
/* At least gcc 3.4.6 on mipsel system needs optimization barrier */
#ifndef __ORCAC__
# define barrier() __asm__ __volatile__("":::"memory")
#else
# define barrier()
#endif

/* You can change LIBBB_DEFAULT_LOGIN_SHELL, but don't use it,
 * use bb_default_login_shell and following defines.
 * If you change LIBBB_DEFAULT_LOGIN_SHELL,
 * don't forget to change increment constant. */
#define LIBBB_DEFAULT_LOGIN_SHELL  "-/bin/sh"

#define ARRAY_SIZE(x) ((size_t)(sizeof(x) / sizeof((x)[0])))
/* ORCA/C will sometimes barf on the expression in ARRAY_SIZE, depending on the element type
 * (e.g. for arrays of structs).  When it does, use ARRAY_SIZE2 instead. */
#define ARRAY_SIZE2(x, elttype) ((size_t)(sizeof(x) / sizeof(elttype)))


/* We redefine ctype macros. Unicode-correct handling of char types
 * can't be done with such byte-oriented operations anyway,
 * we don't lose anything.
 */
#ifndef __ORCAC__
// ORCA/C barfs on (at least) isspace, so we'll just use the standard versions of these.
#undef isalnum
#undef isalpha
#undef isascii
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef toupper
#undef tolower

/* We save ~500 bytes on isdigit alone.
 * BTW, x86 likes (unsigned char) cast more than (unsigned). */

/* These work the same for ASCII and Unicode,
 * assuming no one asks "is this a *Unicode* letter?" using isalpha(letter) */
#define isascii(a) ((unsigned char)(a) <= 0x7f)
#define isdigit(a) ((unsigned char)((a) - '0') <= 9)
#define isupper(a) ((unsigned char)((a) - 'A') <= ('Z' - 'A'))
#define islower(a) ((unsigned char)((a) - 'a') <= ('z' - 'a'))
#define isalpha(a) ((unsigned char)(((a)|0x20) - 'a') <= ('z' - 'a'))
#define isblank(a) ({ unsigned char bb__isblank = (a); bb__isblank == ' ' || bb__isblank == '\t'; })
#define iscntrl(a) ({ unsigned char bb__iscntrl = (a); bb__iscntrl < ' ' || bb__iscntrl == 0x7f; })
/* In POSIX/C locale isspace is only these chars: "\t\n\v\f\r" and space.
 * "\t\n\v\f\r" happen to have ASCII codes 9,10,11,12,13.
 */
#define isspace(a) ({ unsigned char bb__isspace = (a) - 9; bb__isspace == (' ' - 9) || bb__isspace <= (13 - 9); })
// Unsafe wrt NUL: #define ispunct(a) (strchr("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~", (a)) != NULL)
#define ispunct(a) (strchrnul("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~", (a))[0])
// Bigger code: #define isalnum(a) ({ unsigned char bb__isalnum = (a) - '0'; bb__isalnum <= 9 || ((bb__isalnum - ('A' - '0')) & 0xdf) <= 25; })
#define isalnum(a) bb_ascii_isalnum(a)
static ALWAYS_INLINE int bb_ascii_isalnum(unsigned char a)
{
	unsigned char b = a - '0';
	if (b <= 9)
		return (b <= 9);
	b = (a|0x20) - 'a';
	return b <= 'z' - 'a';
}
#define isxdigit(a) bb_ascii_isxdigit(a)
static ALWAYS_INLINE int bb_ascii_isxdigit(unsigned char a)
{
	unsigned char b = a - '0';
	if (b <= 9)
		return (b <= 9);
	b = (a|0x20) - 'a';
	return b <= 'f' - 'a';
}
#define toupper(a) bb_ascii_toupper(a)
static ALWAYS_INLINE unsigned char bb_ascii_toupper(unsigned char a)
{
	unsigned char b = a - 'a';
	if (b <= ('z' - 'a'))
		a -= 'a' - 'A';
	return a;
}
#define tolower(a) bb_ascii_tolower(a)
static ALWAYS_INLINE unsigned char bb_ascii_tolower(unsigned char a)
{
	unsigned char b = a - 'A';
	if (b <= ('Z' - 'A'))
		a += 'a' - 'A';
	return a;
}

/* In ASCII and Unicode, these are likely to be very different.
 * Let's prevent ambiguous usage from the start */
#define isgraph(a) isgraph_is_ambiguous_dont_use(a)
#define isprint(a) isprint_is_ambiguous_dont_use(a)
/* NB: must not treat EOF as isgraph or isprint */
#define isgraph_asciionly(a) ((unsigned)((a) - 0x21) <= 0x7e - 0x21)
#define isprint_asciionly(a) ((unsigned)((a) - 0x20) <= 0x7e - 0x20)
#endif /* !defined(__ORCAC__) */


/* Signal to the parent that it can resume executing after a fork,
 * because the child is about to exec or terminate.
 */
void signal_parent_to_resume(void);
int is_forked_child(void);

/* Get path of current executable */
char *get_exec_path(void);

/* On GNO, we need to use this wrapper because _exit is broken when
 * called from forked child processes. */
#ifdef __GNO__
void _exit_wrapper(int status) NORETURN;
# define _exit(s) _exit_wrapper(s)
#endif

/* Simple unit-testing framework */

typedef void (*bbunit_testfunc)(void);

struct bbunit_listelem {
	const char* name;
	bbunit_testfunc testfunc;
};

void bbunit_registertest(struct bbunit_listelem* test);
void bbunit_settestfailed(void);

#define BBUNIT_DEFINE_TEST(NAME) \
	static void bbunit_##NAME##_test(void); \
	static struct bbunit_listelem bbunit_##NAME##_elem = { \
		.name = #NAME, \
		.testfunc = bbunit_##NAME##_test, \
	}; \
	static void INIT_FUNC bbunit_##NAME##_register(void) \
	{ \
		bbunit_registertest(&bbunit_##NAME##_elem); \
	} \
	static void bbunit_##NAME##_test(void)

/*
 * Both 'goto bbunit_end' and 'break' are here only to get rid
 * of compiler warnings.
 */
#define BBUNIT_ENDTEST \
	do { \
		goto bbunit_end; \
	bbunit_end: \
		break; \
	} while (0)

#define BBUNIT_PRINTASSERTFAIL \
	do { \
		bb_error_msg( \
			"[ERROR] Assertion failed in file %s, line %d", \
			__FILE__, __LINE__); \
	} while (0)

#define BBUNIT_ASSERTION_FAILED \
	do { \
		bbunit_settestfailed(); \
		goto bbunit_end; \
	} while (0)

/*
 * Assertions.
 * For now we only offer assertions which cause tests to fail
 * immediately. In the future 'expects' might be added too -
 * similar to those offered by the gtest framework.
 */
#define BBUNIT_ASSERT_EQ(EXPECTED, ACTUAL) \
	do { \
		if ((EXPECTED) != (ACTUAL)) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] '%s' isn't equal to '%s'", \
						#EXPECTED, #ACTUAL); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_NOTEQ(EXPECTED, ACTUAL) \
	do { \
		if ((EXPECTED) == (ACTUAL)) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] '%s' is equal to '%s'", \
						#EXPECTED, #ACTUAL); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_NOTNULL(PTR) \
	do { \
		if ((PTR) == NULL) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] '%s' is NULL!", #PTR); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_NULL(PTR) \
	do { \
		if ((PTR) != NULL) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] '%s' is not NULL!", #PTR); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_FALSE(STATEMENT) \
	do { \
		if ((STATEMENT)) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] Statement '%s' evaluated to true!", \
								#STATEMENT); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_TRUE(STATEMENT) \
	do { \
		if (!(STATEMENT)) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] Statement '%s' evaluated to false!", \
					#STATEMENT); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_STREQ(STR1, STR2) \
	do { \
		if (strcmp(STR1, STR2) != 0) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] Strings '%s' and '%s' " \
					"are not the same", STR1, STR2); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)

#define BBUNIT_ASSERT_STRNOTEQ(STR1, STR2) \
	do { \
		if (strcmp(STR1, STR2) == 0) { \
			BBUNIT_PRINTASSERTFAIL; \
			bb_error_msg("[ERROR] Strings '%s' and '%s' " \
					"are the same, but were " \
					"expected to differ", STR1, STR2); \
			BBUNIT_ASSERTION_FAILED; \
		} \
	} while (0)


POP_SAVED_FUNCTION_VISIBILITY

#endif
