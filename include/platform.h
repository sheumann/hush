/* vi: set sw=4 ts=4: */
/*
 * Copyright 2006, Bernhard Reutner-Fischer
 *
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */
#ifndef BB_PLATFORM_H
#define BB_PLATFORM_H 1

#include "autoconf.h"

/* Convenience macros to test the version of gcc. */
#undef __GNUC_PREREQ
#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
		((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

/* __restrict is known in EGCS 1.2 and above. */
#if !__GNUC_PREREQ(2,92)
# ifndef __restrict
#  define __restrict
# endif
#endif

#if !__GNUC_PREREQ(2,7)
# ifndef __attribute__
#  define __attribute__(x)
# endif
#endif

#undef inline
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 199901L
/* it's a keyword */
#elif __GNUC_PREREQ(2,7)
# define inline __inline__
#else
# define inline
#endif

#ifndef __const
# define __const const
#endif

#define UNUSED_PARAM __attribute__ ((__unused__))
#define NORETURN __attribute__ ((__noreturn__))
/* "The malloc attribute is used to tell the compiler that a function
 * may be treated as if any non-NULL pointer it returns cannot alias
 * any other pointer valid when the function returns. This will often
 * improve optimization. Standard functions with this property include
 * malloc and calloc. realloc-like functions have this property as long
 * as the old pointer is never referred to (including comparing it
 * to the new pointer) after the function returns a non-NULL value."
 */
#define RETURNS_MALLOC __attribute__ ((malloc))
#define PACKED __attribute__ ((__packed__))
#define ALIGNED(m) __attribute__ ((__aligned__(m)))

/* __NO_INLINE__: some gcc's do not honor inlining! :( */
#if __GNUC_PREREQ(3,0) && !defined(__NO_INLINE__)
# define ALWAYS_INLINE __attribute__ ((always_inline)) inline
/* I've seen a toolchain where I needed __noinline__ instead of noinline */
# define NOINLINE      __attribute__((__noinline__))
# if !ENABLE_WERROR
#  define DEPRECATED __attribute__ ((__deprecated__))
#  define UNUSED_PARAM_RESULT __attribute__ ((warn_unused_result))
# else
#  define DEPRECATED
#  define UNUSED_PARAM_RESULT
# endif
#else
# ifdef __ORCAC__
#  define ALWAYS_INLINE
# else
#  define ALWAYS_INLINE inline
# endif
# define NOINLINE
# define DEPRECATED
# define UNUSED_PARAM_RESULT
#endif

/* used by unit test machinery to run registration functions before calling main() */
#define INIT_FUNC __attribute__ ((constructor))

/* -fwhole-program makes all symbols local. The attribute externally_visible
 * forces a symbol global.  */
#if __GNUC_PREREQ(4,1)
# define EXTERNALLY_VISIBLE __attribute__(( visibility("default") ))
//__attribute__ ((__externally_visible__))
#else
# define EXTERNALLY_VISIBLE
#endif

/* At 4.4 gcc become much more anal about this, need to use "aliased" types */
#if __GNUC_PREREQ(4,4)
# define FIX_ALIASING __attribute__((__may_alias__))
#else
# define FIX_ALIASING
#endif

/* We use __extension__ in some places to suppress -pedantic warnings
 * about GCC extensions.  This feature didn't work properly before
 * gcc 2.8.  */
#if !__GNUC_PREREQ(2,8)
# ifndef __extension__
#  define __extension__
# endif
#endif

/* FAST_FUNC is a qualifier which (possibly) makes function call faster
 * and/or smaller by using modified ABI. It is usually only needed
 * on non-static, busybox internal functions. Recent versions of gcc
 * optimize statics automatically. FAST_FUNC on static is required
 * only if you need to match a function pointer's type */
#if __GNUC_PREREQ(3,0) && defined(i386) /* || defined(__x86_64__)? */
/* stdcall makes callee to pop arguments from stack, not caller */
# define FAST_FUNC __attribute__((regparm(3),stdcall))
/* #elif ... - add your favorite arch today! */
#else
# define FAST_FUNC
#endif

/* Make all declarations hidden (-fvisibility flag only affects definitions) */
/* (don't include system headers after this until corresponding pop!) */
#if __GNUC_PREREQ(4,1) && !defined(__CYGWIN__)
# define PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN _Pragma("GCC visibility push(hidden)")
# define POP_SAVED_FUNCTION_VISIBILITY              _Pragma("GCC visibility pop")
#else
# define PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN
# define POP_SAVED_FUNCTION_VISIBILITY
#endif

/* gcc-2.95 had no va_copy but only __va_copy. */
#if !__GNUC_PREREQ(3,0)
# include <stdarg.h>
# if !defined va_copy && defined __va_copy
#  define va_copy(d,s) __va_copy((d),(s))
# endif
#endif


#ifndef __ORCAC__
# include <inttypes.h>
#else
# include <limits.h>
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long intmax_t;
typedef unsigned long uintmax_t;
# define PRIxMAX "lx"
# define PRIdMAX "ld"
# define PRIuMAX "lu"
# define UINTMAX_MAX ULONG_MAX
typedef unsigned long uintptr_t;
#endif


/* ---- Size-saving "small" ints (arch-dependent) ----------- */

#if defined(i386) || defined(__x86_64__) || defined(__mips__) || defined(__cris__) || defined(__ORCAC__)
/* add other arches which benefit from this... */
typedef signed char smallint;
typedef unsigned char smalluint;
#else
/* for arches where byte accesses generate larger code: */
typedef int smallint;
typedef unsigned smalluint;
#endif

/* ISO C Standard:  7.16  Boolean type and values  <stdbool.h> */
#if (defined __digital__ && defined __unix__) || defined(__ORCAC__)
/* old system without (proper) C99 support */
# define bool smalluint
#else
/* modern system, so use it */
# include <stdbool.h>
#endif


/*----- Kernel versioning ------------------------------------*/

#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#ifdef __UCLIBC__
# define UCLIBC_VERSION KERNEL_VERSION(__UCLIBC_MAJOR__, __UCLIBC_MINOR__, __UCLIBC_SUBLEVEL__)
#else
# define UCLIBC_VERSION 0
#endif


/* ---- Miscellaneous --------------------------------------- */

#if defined __GLIBC__ \
 || defined __UCLIBC__ \
 || defined __dietlibc__ \
 || defined __BIONIC__ \
 || defined _NEWLIB_VERSION
# include <features.h>
#endif

/* Define bb_setpgrp */
#if defined(__digital__) && defined(__unix__) || defined(__GNO__)
/* use legacy setpgrp(pid_t, pid_t) for now.  move to platform.c */
# define bb_setpgrp() do { pid_t __me = getpid(); setpgrp(__me, __me); } while (0)
#else
# define bb_setpgrp() setpgrp()
#endif

/* fdprintf is more readable, we used it before dprintf was standardized */
#include <unistd.h>
#define fdprintf dprintf

/* Useful for defeating gcc's alignment of "char message[]"-like data */
#if !defined(__s390__)
    /* on s390[x], non-word-aligned data accesses require larger code */
# define ALIGN1 __attribute__((aligned(1)))
# define ALIGN2 __attribute__((aligned(2)))
# define ALIGN4 __attribute__((aligned(4)))
#else
/* Arches which MUST have 2 or 4 byte alignment for everything are here */
# define ALIGN1
# define ALIGN2
# define ALIGN4
#endif

/*
 * For 0.9.29 and svn, __ARCH_USE_MMU__ indicates no-mmu reliably.
 * For earlier versions there is no reliable way to check if we are building
 * for a mmu-less system.
 */
#if ENABLE_NOMMU || \
    (defined __UCLIBC__ && \
     UCLIBC_VERSION > KERNEL_VERSION(0, 9, 28) && \
     !defined __ARCH_USE_MMU__)
# define BB_MMU 0
# define USE_FOR_NOMMU(x) x
# define USE_FOR_MMU(x)
#else
# define BB_MMU 1
# define USE_FOR_NOMMU(x)
# define USE_FOR_MMU(x) x
#endif

#if defined(__digital__) && defined(__unix__)
# include <standards.h>
# include <inttypes.h>
# define PRIu32 "u"
# if !defined ADJ_OFFSET_SINGLESHOT && defined MOD_CLKA && defined MOD_OFFSET
#  define ADJ_OFFSET_SINGLESHOT (MOD_CLKA | MOD_OFFSET)
# endif
# if !defined ADJ_FREQUENCY && defined MOD_FREQUENCY
#  define ADJ_FREQUENCY MOD_FREQUENCY
# endif
# if !defined ADJ_TIMECONST && defined MOD_TIMECONST
#  define ADJ_TIMECONST MOD_TIMECONST
# endif
# if !defined ADJ_TICK && defined MOD_CLKB
#  define ADJ_TICK MOD_CLKB
# endif
#endif

#if defined(__CYGWIN__)
# define MAXSYMLINKS SYMLOOP_MAX
#endif

#if defined(ANDROID) || defined(__ANDROID__)
# define BB_ADDITIONAL_PATH ":/system/sbin:/system/bin:/system/xbin"
# define SYS_ioprio_set __NR_ioprio_set
# define SYS_ioprio_get __NR_ioprio_get
#endif


/* ---- Who misses what? ------------------------------------ */

/* Assume all these functions and header files exist by default.
 * Platforms where it is not true will #undef them below.
 */
#define HAVE_CLEARENV 1
#define HAVE_FDATASYNC 1
#define HAVE_DPRINTF 1
#define HAVE_MEMRCHR 1
#define HAVE_MKDTEMP 1
#define HAVE_PTSNAME_R 1
#define HAVE_SETBIT 1
#define HAVE_SIGHANDLER_T 1
#define HAVE_STPCPY 1
#define HAVE_STRCASESTR 1
#define HAVE_STRCHRNUL 1
#define HAVE_STRSEP 1
#define HAVE_STRSIGNAL 1
#define HAVE_STRVERSCMP 1
#define HAVE_VASPRINTF 1
#define HAVE_USLEEP 1
#define HAVE_UNLOCKED_STDIO 1
#define HAVE_UNLOCKED_LINE_OPS 1
#define HAVE_GETLINE 1
#define HAVE_XTABS 1
#define HAVE_MNTENT_H 1
#define HAVE_NET_ETHERNET_H 1
#define HAVE_SYS_STATFS_H 1

#if defined(__UCLIBC__)
# if UCLIBC_VERSION < KERNEL_VERSION(0, 9, 32)
#  undef HAVE_STRVERSCMP
# endif
# if UCLIBC_VERSION >= KERNEL_VERSION(0, 9, 30)
#  ifndef __UCLIBC_SUSV3_LEGACY__
#   undef HAVE_USLEEP
#  endif
# endif
#endif

#if defined(__WATCOMC__)
# undef HAVE_DPRINTF
# undef HAVE_GETLINE
# undef HAVE_MEMRCHR
# undef HAVE_MKDTEMP
# undef HAVE_SETBIT
# undef HAVE_STPCPY
# undef HAVE_STRCASESTR
# undef HAVE_STRCHRNUL
# undef HAVE_STRSEP
# undef HAVE_STRSIGNAL
# undef HAVE_STRVERSCMP
# undef HAVE_VASPRINTF
# undef HAVE_UNLOCKED_STDIO
# undef HAVE_UNLOCKED_LINE_OPS
# undef HAVE_NET_ETHERNET_H
#endif

#if defined(__CYGWIN__)
# undef HAVE_CLEARENV
# undef HAVE_FDPRINTF
# undef HAVE_MEMRCHR
# undef HAVE_PTSNAME_R
# undef HAVE_STRVERSCMP
# undef HAVE_UNLOCKED_LINE_OPS
#endif

/* These BSD-derived OSes share many similarities */
#if (defined __digital__ && defined __unix__) \
 || defined __APPLE__ \
 || defined __OpenBSD__ || defined __NetBSD__
# undef HAVE_CLEARENV
# undef HAVE_FDATASYNC
# undef HAVE_GETLINE
# undef HAVE_MNTENT_H
# undef HAVE_PTSNAME_R
# undef HAVE_SYS_STATFS_H
# undef HAVE_SIGHANDLER_T
# undef HAVE_STRVERSCMP
# undef HAVE_XTABS
# undef HAVE_DPRINTF
# undef HAVE_UNLOCKED_STDIO
# undef HAVE_UNLOCKED_LINE_OPS
#endif

#if defined(__dietlibc__)
# undef HAVE_STRCHRNUL
#endif

#if defined(__APPLE__)
# undef HAVE_STRCHRNUL
#endif

#if defined(__FreeBSD__)
# undef HAVE_CLEARENV
# undef HAVE_FDATASYNC
# undef HAVE_MNTENT_H
# undef HAVE_PTSNAME_R
# undef HAVE_SYS_STATFS_H
# undef HAVE_SIGHANDLER_T
# undef HAVE_STRVERSCMP
# undef HAVE_XTABS
# undef HAVE_UNLOCKED_LINE_OPS
# include <osreldate.h>
# if __FreeBSD_version < 1000029
#  undef HAVE_STRCHRNUL /* FreeBSD added strchrnul() between 1000028 and 1000029 */
# endif
#endif

#if defined(__NetBSD__)
# define HAVE_GETLINE 1  /* Recent NetBSD versions have getline() */
#endif

#if defined(__digital__) && defined(__unix__)
# undef HAVE_STPCPY
#endif

#if defined(ANDROID) || defined(__ANDROID__)
# undef HAVE_DPRINTF
# undef HAVE_GETLINE
# undef HAVE_STPCPY
# undef HAVE_STRCHRNUL
# undef HAVE_STRVERSCMP
# undef HAVE_UNLOCKED_LINE_OPS
# undef HAVE_NET_ETHERNET_H
#endif

#if defined(__GNO__)
# undef HAVE_CLEARENV
# undef HAVE_FDATASYNC
# undef HAVE_DPRINTF
# undef HAVE_MEMRCHR
# undef HAVE_MKDTEMP
# undef HAVE_PTSNAME_R
# undef HAVE_SIGHANDLER_T
# undef HAVE_STPCPY
# undef HAVE_STRCASESTR
# undef HAVE_STRCHRNUL
# undef HAVE_STRSIGNAL
# undef HAVE_VASPRINTF
# undef HAVE_GETLINE
# undef HAVE_XTABS
# undef HAVE_UNLOCKED_STDIO
# undef HAVE_UNLOCKED_LINE_OPS
# undef HAVE_MNTENT_H
# undef HAVE_NET_ETHERNET_H
# undef HAVE_SYS_STATFS_H
#endif

/*
 * Now, define prototypes for all the functions defined in platform.c
 * These must come after all the HAVE_* macros are defined (or not)
 */

#ifndef HAVE_DPRINTF
extern int dprintf(int fd, const char *format, ...);
#endif

#ifndef HAVE_MEMRCHR
extern void *memrchr(const void *s, int c, size_t n) FAST_FUNC;
#endif

#ifndef HAVE_MKDTEMP
extern char *mkdtemp(char *template) FAST_FUNC;
#endif

#ifndef HAVE_SETBIT
# define setbit(a, b)  ((a)[(b) >> 3] |= 1 << ((b) & 7))
# define clrbit(a, b)  ((a)[(b) >> 3] &= ~(1 << ((b) & 7)))
#endif

#ifndef HAVE_SIGHANDLER_T
# ifndef __GNO__
typedef void (*sighandler_t)(int);
# else
typedef void (*sighandler_t)(int, int);
# endif
#endif

#ifndef HAVE_STPCPY
extern char *stpcpy(char *p, const char *to_add) FAST_FUNC;
#endif

#ifndef HAVE_STRCASESTR
extern char *strcasestr(const char *s, const char *pattern) FAST_FUNC;
#endif

#ifndef HAVE_STRCHRNUL
extern char *strchrnul(const char *s, int c) FAST_FUNC;
#endif

#ifndef HAVE_STRSEP
extern char *strsep(char **stringp, const char *delim) FAST_FUNC;
#endif

#ifndef HAVE_STRSIGNAL
/* Not exactly the same: instead of "Stopped" it shows "STOP" etc */
# define strsignal(sig) get_signame(sig)
#endif

#ifndef HAVE_USLEEP
extern int usleep(unsigned) FAST_FUNC;
#endif

#ifndef HAVE_VASPRINTF
extern int vasprintf(char **string_ptr, const char *format, va_list p) FAST_FUNC;
#endif

#ifndef HAVE_GETLINE
# include <stdio.h> /* for FILE */
# include <sys/types.h> /* size_t */
extern ssize_t getline(char **lineptr, size_t *n, FILE *stream) FAST_FUNC;
#endif


#ifdef __GNO__
// GNO mkdir doesn't take a mode parameter
# define mkdir(path, mode) mkdir(path)
#endif

#if NSIG <= 32
typedef unsigned long sigmask_t;
#else
typedef unsigned long long sigmask_t;
#endif

#ifdef __GNO__
# include <sys/wait.h>
typedef union wait wait_status_t;
#else
typedef int wait_status_t;
#endif

#endif
