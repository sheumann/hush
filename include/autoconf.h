/*
 * Automatically generated, but now edited manually
 * Busybox version: 1.22.1
 */
#define AUTOCONF_TIMESTAMP "2014-10-26 09:24:54 CDT"

#define CONFIG_HAVE_DOT_CONFIG 1
#define ENABLE_HAVE_DOT_CONFIG 1
#ifdef MAKE_SUID
# define IF_HAVE_DOT_CONFIG(x) x "CONFIG_HAVE_DOT_CONFIG"
#else
# define IF_HAVE_DOT_CONFIG(x) x
#endif
#define IF_NOT_HAVE_DOT_CONFIG(x)

/*
 * Busybox Settings
 */

/*
 * General Configuration
 */
#undef CONFIG_DESKTOP
#define ENABLE_DESKTOP 0
#define IF_DESKTOP(x)
#define IF_NOT_DESKTOP(x) x
#undef CONFIG_EXTRA_COMPAT
#define ENABLE_EXTRA_COMPAT 0
#define IF_EXTRA_COMPAT(x)
#define IF_NOT_EXTRA_COMPAT(x) x
#define CONFIG_INCLUDE_SUSv2 1
#define ENABLE_INCLUDE_SUSv2 1
#ifdef MAKE_SUID
# define IF_INCLUDE_SUSv2(x) x "CONFIG_INCLUDE_SUSv2"
#else
# define IF_INCLUDE_SUSv2(x) x
#endif
#define IF_NOT_INCLUDE_SUSv2(x)
#define CONFIG_USE_PORTABLE_CODE 1
#define ENABLE_USE_PORTABLE_CODE 1
#ifdef MAKE_SUID
# define IF_USE_PORTABLE_CODE(x) x "CONFIG_USE_PORTABLE_CODE"
#else
# define IF_USE_PORTABLE_CODE(x) x
#endif
#define IF_NOT_USE_PORTABLE_CODE(x)
#undef CONFIG_PLATFORM_LINUX
#define ENABLE_PLATFORM_LINUX 0
#define IF_PLATFORM_LINUX(x)
#define IF_NOT_PLATFORM_LINUX(x) x
#define CONFIG_FEATURE_BUFFERS_USE_MALLOC 1
#define ENABLE_FEATURE_BUFFERS_USE_MALLOC 1
#ifdef MAKE_SUID
# define IF_FEATURE_BUFFERS_USE_MALLOC(x) x "CONFIG_FEATURE_BUFFERS_USE_MALLOC"
#else
# define IF_FEATURE_BUFFERS_USE_MALLOC(x) x
#endif
#define IF_NOT_FEATURE_BUFFERS_USE_MALLOC(x)
#undef CONFIG_FEATURE_BUFFERS_GO_ON_STACK
#define ENABLE_FEATURE_BUFFERS_GO_ON_STACK 0
#define IF_FEATURE_BUFFERS_GO_ON_STACK(x)
#define IF_NOT_FEATURE_BUFFERS_GO_ON_STACK(x) x
#undef CONFIG_FEATURE_BUFFERS_GO_IN_BSS
#define ENABLE_FEATURE_BUFFERS_GO_IN_BSS 0
#define IF_FEATURE_BUFFERS_GO_IN_BSS(x)
#define IF_NOT_FEATURE_BUFFERS_GO_IN_BSS(x) x
#define CONFIG_SHOW_USAGE 1
#define ENABLE_SHOW_USAGE 1
#ifdef MAKE_SUID
# define IF_SHOW_USAGE(x) x "CONFIG_SHOW_USAGE"
#else
# define IF_SHOW_USAGE(x) x
#endif
#define IF_NOT_SHOW_USAGE(x)
#define CONFIG_FEATURE_VERBOSE_USAGE 1
#define ENABLE_FEATURE_VERBOSE_USAGE 1
#ifdef MAKE_SUID
# define IF_FEATURE_VERBOSE_USAGE(x) x "CONFIG_FEATURE_VERBOSE_USAGE"
#else
# define IF_FEATURE_VERBOSE_USAGE(x) x
#endif
#define IF_NOT_FEATURE_VERBOSE_USAGE(x)
#undef CONFIG_FEATURE_COMPRESS_USAGE
#define ENABLE_FEATURE_COMPRESS_USAGE 0
#define IF_FEATURE_COMPRESS_USAGE(x)
#define IF_NOT_FEATURE_COMPRESS_USAGE(x) x
#undef CONFIG_FEATURE_INSTALLER
#define ENABLE_FEATURE_INSTALLER 0
#define IF_FEATURE_INSTALLER(x)
#define IF_NOT_FEATURE_INSTALLER(x) x
#undef CONFIG_INSTALL_NO_USR
#define ENABLE_INSTALL_NO_USR 0
#define IF_INSTALL_NO_USR(x)
#define IF_NOT_INSTALL_NO_USR(x) x
#undef CONFIG_LOCALE_SUPPORT
#define ENABLE_LOCALE_SUPPORT 0
#define IF_LOCALE_SUPPORT(x)
#define IF_NOT_LOCALE_SUPPORT(x) x
#define CONFIG_UNICODE_SUPPORT 1
#define ENABLE_UNICODE_SUPPORT 1
#ifdef MAKE_SUID
# define IF_UNICODE_SUPPORT(x) x "CONFIG_UNICODE_SUPPORT"
#else
# define IF_UNICODE_SUPPORT(x) x
#endif
#define IF_NOT_UNICODE_SUPPORT(x)
#undef CONFIG_UNICODE_USING_LOCALE
#define ENABLE_UNICODE_USING_LOCALE 0
#define IF_UNICODE_USING_LOCALE(x)
#define IF_NOT_UNICODE_USING_LOCALE(x) x
#undef CONFIG_FEATURE_CHECK_UNICODE_IN_ENV
#define ENABLE_FEATURE_CHECK_UNICODE_IN_ENV 0
#define IF_FEATURE_CHECK_UNICODE_IN_ENV(x)
#define IF_NOT_FEATURE_CHECK_UNICODE_IN_ENV(x) x
#define CONFIG_SUBST_WCHAR 63
#define ENABLE_SUBST_WCHAR 1
#ifdef MAKE_SUID
# define IF_SUBST_WCHAR(x) x "CONFIG_SUBST_WCHAR"
#else
# define IF_SUBST_WCHAR(x) x
#endif
#define IF_NOT_SUBST_WCHAR(x)
#define CONFIG_LAST_SUPPORTED_WCHAR 767
#define ENABLE_LAST_SUPPORTED_WCHAR 1
#ifdef MAKE_SUID
# define IF_LAST_SUPPORTED_WCHAR(x) x "CONFIG_LAST_SUPPORTED_WCHAR"
#else
# define IF_LAST_SUPPORTED_WCHAR(x) x
#endif
#define IF_NOT_LAST_SUPPORTED_WCHAR(x)
#undef CONFIG_UNICODE_COMBINING_WCHARS
#define ENABLE_UNICODE_COMBINING_WCHARS 0
#define IF_UNICODE_COMBINING_WCHARS(x)
#define IF_NOT_UNICODE_COMBINING_WCHARS(x) x
#undef CONFIG_UNICODE_WIDE_WCHARS
#define ENABLE_UNICODE_WIDE_WCHARS 0
#define IF_UNICODE_WIDE_WCHARS(x)
#define IF_NOT_UNICODE_WIDE_WCHARS(x) x
#undef CONFIG_UNICODE_BIDI_SUPPORT
#define ENABLE_UNICODE_BIDI_SUPPORT 0
#define IF_UNICODE_BIDI_SUPPORT(x)
#define IF_NOT_UNICODE_BIDI_SUPPORT(x) x
#undef CONFIG_UNICODE_NEUTRAL_TABLE
#define ENABLE_UNICODE_NEUTRAL_TABLE 0
#define IF_UNICODE_NEUTRAL_TABLE(x)
#define IF_NOT_UNICODE_NEUTRAL_TABLE(x) x
#undef CONFIG_UNICODE_PRESERVE_BROKEN
#define ENABLE_UNICODE_PRESERVE_BROKEN 0
#define IF_UNICODE_PRESERVE_BROKEN(x)
#define IF_NOT_UNICODE_PRESERVE_BROKEN(x) x
#undef CONFIG_LONG_OPTS
#define ENABLE_LONG_OPTS 0
#define IF_LONG_OPTS(x) 
#define IF_NOT_LONG_OPTS(x) x
#define CONFIG_FEATURE_DEVPTS 1
#define ENABLE_FEATURE_DEVPTS 1
#ifdef MAKE_SUID
# define IF_FEATURE_DEVPTS(x) x "CONFIG_FEATURE_DEVPTS"
#else
# define IF_FEATURE_DEVPTS(x) x
#endif
#define IF_NOT_FEATURE_DEVPTS(x)
#undef CONFIG_FEATURE_CLEAN_UP
#define ENABLE_FEATURE_CLEAN_UP 0
#define IF_FEATURE_CLEAN_UP(x)
#define IF_NOT_FEATURE_CLEAN_UP(x) x
#define CONFIG_FEATURE_UTMP 1
#define ENABLE_FEATURE_UTMP 1
#ifdef MAKE_SUID
# define IF_FEATURE_UTMP(x) x "CONFIG_FEATURE_UTMP"
#else
# define IF_FEATURE_UTMP(x) x
#endif
#define IF_NOT_FEATURE_UTMP(x)
#define CONFIG_FEATURE_WTMP 1
#define ENABLE_FEATURE_WTMP 1
#ifdef MAKE_SUID
# define IF_FEATURE_WTMP(x) x "CONFIG_FEATURE_WTMP"
#else
# define IF_FEATURE_WTMP(x) x
#endif
#define IF_NOT_FEATURE_WTMP(x)
#define CONFIG_FEATURE_PIDFILE 1
#define ENABLE_FEATURE_PIDFILE 1
#ifdef MAKE_SUID
# define IF_FEATURE_PIDFILE(x) x "CONFIG_FEATURE_PIDFILE"
#else
# define IF_FEATURE_PIDFILE(x) x
#endif
#define IF_NOT_FEATURE_PIDFILE(x)
#define CONFIG_PID_FILE_PATH "/var/run"
#define ENABLE_PID_FILE_PATH 1
#ifdef MAKE_SUID
# define IF_PID_FILE_PATH(x) x "CONFIG_PID_FILE_PATH"
#else
# define IF_PID_FILE_PATH(x) x
#endif
#define IF_NOT_PID_FILE_PATH(x)
#define CONFIG_FEATURE_SUID 1
#define ENABLE_FEATURE_SUID 1
#ifdef MAKE_SUID
# define IF_FEATURE_SUID(x) x "CONFIG_FEATURE_SUID"
#else
# define IF_FEATURE_SUID(x) x
#endif
#define IF_NOT_FEATURE_SUID(x)
#define CONFIG_FEATURE_SUID_CONFIG 1
#define ENABLE_FEATURE_SUID_CONFIG 1
#ifdef MAKE_SUID
# define IF_FEATURE_SUID_CONFIG(x) x "CONFIG_FEATURE_SUID_CONFIG"
#else
# define IF_FEATURE_SUID_CONFIG(x) x
#endif
#define IF_NOT_FEATURE_SUID_CONFIG(x)
#define CONFIG_FEATURE_SUID_CONFIG_QUIET 1
#define ENABLE_FEATURE_SUID_CONFIG_QUIET 1
#ifdef MAKE_SUID
# define IF_FEATURE_SUID_CONFIG_QUIET(x) x "CONFIG_FEATURE_SUID_CONFIG_QUIET"
#else
# define IF_FEATURE_SUID_CONFIG_QUIET(x) x
#endif
#define IF_NOT_FEATURE_SUID_CONFIG_QUIET(x)
#undef CONFIG_SELINUX
#define ENABLE_SELINUX 0
#define IF_SELINUX(x)
#define IF_NOT_SELINUX(x) x
#undef CONFIG_FEATURE_PREFER_APPLETS
#define ENABLE_FEATURE_PREFER_APPLETS 0
#define IF_FEATURE_PREFER_APPLETS(x)
#define IF_NOT_FEATURE_PREFER_APPLETS(x) x
#ifdef MAKE_SUID
# define IF_BUSYBOX_EXEC_PATH(x) x "CONFIG_BUSYBOX_EXEC_PATH"
#else
# define IF_BUSYBOX_EXEC_PATH(x) x
#endif
#define IF_NOT_BUSYBOX_EXEC_PATH(x)
#undef CONFIG_FEATURE_SYSLOG
#define ENABLE_FEATURE_SYSLOG 0
#define IF_FEATURE_SYSLOG(x)
#define IF_NOT_FEATURE_SYSLOG(x) x
#undef CONFIG_FEATURE_HAVE_RPC
#define ENABLE_FEATURE_HAVE_RPC 0
#define IF_FEATURE_HAVE_RPC(x)
#define IF_NOT_FEATURE_HAVE_RPC(x) x

/*
 * Build Options
 */
#undef CONFIG_STATIC
#define ENABLE_STATIC 0
#define IF_STATIC(x)
#define IF_NOT_STATIC(x) x
#undef CONFIG_PIE
#define ENABLE_PIE 0
#define IF_PIE(x)
#define IF_NOT_PIE(x) x
#define CONFIG_NOMMU 1
#define ENABLE_NOMMU 1
#ifdef MAKE_SUID
# define IF_NOMMU(x) x "CONFIG_NOMMU"
#else
# define IF_NOMMU(x) x
#endif
#define IF_NOT_NOMMU(x)
#undef CONFIG_BUILD_LIBBUSYBOX
#define ENABLE_BUILD_LIBBUSYBOX 0
#define IF_BUILD_LIBBUSYBOX(x)
#define IF_NOT_BUILD_LIBBUSYBOX(x) x
#undef CONFIG_FEATURE_INDIVIDUAL
#define ENABLE_FEATURE_INDIVIDUAL 0
#define IF_FEATURE_INDIVIDUAL(x)
#define IF_NOT_FEATURE_INDIVIDUAL(x) x
#undef CONFIG_FEATURE_SHARED_BUSYBOX
#define ENABLE_FEATURE_SHARED_BUSYBOX 0
#define IF_FEATURE_SHARED_BUSYBOX(x)
#define IF_NOT_FEATURE_SHARED_BUSYBOX(x) x
#undef CONFIG_LFS
#define ENABLE_LFS 0
#define IF_LFS(x)
#define IF_NOT_LFS(x) x
#define CONFIG_CROSS_COMPILER_PREFIX ""
#define ENABLE_CROSS_COMPILER_PREFIX 1
#ifdef MAKE_SUID
# define IF_CROSS_COMPILER_PREFIX(x) x "CONFIG_CROSS_COMPILER_PREFIX"
#else
# define IF_CROSS_COMPILER_PREFIX(x) x
#endif
#define IF_NOT_CROSS_COMPILER_PREFIX(x)
#define CONFIG_SYSROOT ""
#define ENABLE_SYSROOT 1
#ifdef MAKE_SUID
# define IF_SYSROOT(x) x "CONFIG_SYSROOT"
#else
# define IF_SYSROOT(x) x
#endif
#define IF_NOT_SYSROOT(x)
#define CONFIG_EXTRA_CFLAGS ""
#define ENABLE_EXTRA_CFLAGS 1
#ifdef MAKE_SUID
# define IF_EXTRA_CFLAGS(x) x "CONFIG_EXTRA_CFLAGS"
#else
# define IF_EXTRA_CFLAGS(x) x
#endif
#define IF_NOT_EXTRA_CFLAGS(x)
#define CONFIG_EXTRA_LDFLAGS ""
#define ENABLE_EXTRA_LDFLAGS 1
#ifdef MAKE_SUID
# define IF_EXTRA_LDFLAGS(x) x "CONFIG_EXTRA_LDFLAGS"
#else
# define IF_EXTRA_LDFLAGS(x) x
#endif
#define IF_NOT_EXTRA_LDFLAGS(x)
#define CONFIG_EXTRA_LDLIBS ""
#define ENABLE_EXTRA_LDLIBS 1
#ifdef MAKE_SUID
# define IF_EXTRA_LDLIBS(x) x "CONFIG_EXTRA_LDLIBS"
#else
# define IF_EXTRA_LDLIBS(x) x
#endif
#define IF_NOT_EXTRA_LDLIBS(x)

/*
 * Debugging Options
 */
#undef CONFIG_DEBUG
#define ENABLE_DEBUG 0
#define IF_DEBUG(x)
#define IF_NOT_DEBUG(x) x
#undef CONFIG_DEBUG_PESSIMIZE
#define ENABLE_DEBUG_PESSIMIZE 0
#define IF_DEBUG_PESSIMIZE(x)
#define IF_NOT_DEBUG_PESSIMIZE(x) x
#undef CONFIG_WERROR
#define ENABLE_WERROR 0
#define IF_WERROR(x)
#define IF_NOT_WERROR(x) x
#define CONFIG_NO_DEBUG_LIB 1
#define ENABLE_NO_DEBUG_LIB 1
#ifdef MAKE_SUID
# define IF_NO_DEBUG_LIB(x) x "CONFIG_NO_DEBUG_LIB"
#else
# define IF_NO_DEBUG_LIB(x) x
#endif
#define IF_NOT_NO_DEBUG_LIB(x)
#undef CONFIG_DMALLOC
#define ENABLE_DMALLOC 0
#define IF_DMALLOC(x)
#define IF_NOT_DMALLOC(x) x
#undef CONFIG_EFENCE
#define ENABLE_EFENCE 0
#define IF_EFENCE(x)
#define IF_NOT_EFENCE(x) x

/*
 * Busybox Library Tuning
 */
#undef CONFIG_FEATURE_ETC_NETWORKS
#define ENABLE_FEATURE_ETC_NETWORKS 0
#define IF_FEATURE_ETC_NETWORKS(x)
#define IF_NOT_FEATURE_ETC_NETWORKS(x) x
#undef CONFIG_FEATURE_USE_TERMIOS
#define ENABLE_FEATURE_USE_TERMIOS 0
#define IF_FEATURE_USE_TERMIOS(x)
#define IF_NOT_FEATURE_USE_TERMIOS(x) x
#define CONFIG_FEATURE_EDITING 1
#define ENABLE_FEATURE_EDITING 1
#ifdef MAKE_SUID
# define IF_FEATURE_EDITING(x) x "CONFIG_FEATURE_EDITING"
#else
# define IF_FEATURE_EDITING(x) x
#endif
#define IF_NOT_FEATURE_EDITING(x)
#define CONFIG_FEATURE_EDITING_MAX_LEN 1024
#define ENABLE_FEATURE_EDITING_MAX_LEN 1
#ifdef MAKE_SUID
# define IF_FEATURE_EDITING_MAX_LEN(x) x "CONFIG_FEATURE_EDITING_MAX_LEN"
#else
# define IF_FEATURE_EDITING_MAX_LEN(x) x
#endif
#define IF_NOT_FEATURE_EDITING_MAX_LEN(x)
#undef CONFIG_FEATURE_EDITING_VI
#define ENABLE_FEATURE_EDITING_VI 0
#define IF_FEATURE_EDITING_VI(x)
#define IF_NOT_FEATURE_EDITING_VI(x) x
#define CONFIG_FEATURE_EDITING_HISTORY 255
#define ENABLE_FEATURE_EDITING_HISTORY 1
#ifdef MAKE_SUID
# define IF_FEATURE_EDITING_HISTORY(x) x "CONFIG_FEATURE_EDITING_HISTORY"
#else
# define IF_FEATURE_EDITING_HISTORY(x) x
#endif
#define IF_NOT_FEATURE_EDITING_HISTORY(x)
#define CONFIG_FEATURE_EDITING_SAVEHISTORY 1
#define ENABLE_FEATURE_EDITING_SAVEHISTORY 1
#ifdef MAKE_SUID
# define IF_FEATURE_EDITING_SAVEHISTORY(x) x "CONFIG_FEATURE_EDITING_SAVEHISTORY"
#else
# define IF_FEATURE_EDITING_SAVEHISTORY(x) x
#endif
#define IF_NOT_FEATURE_EDITING_SAVEHISTORY(x)
#undef CONFIG_FEATURE_EDITING_SAVE_ON_EXIT
#define ENABLE_FEATURE_EDITING_SAVE_ON_EXIT 0
#define IF_FEATURE_EDITING_SAVE_ON_EXIT(x)
#define IF_NOT_FEATURE_EDITING_SAVE_ON_EXIT(x) x
#define CONFIG_FEATURE_REVERSE_SEARCH 1
#define ENABLE_FEATURE_REVERSE_SEARCH 1
#ifdef MAKE_SUID
# define IF_FEATURE_REVERSE_SEARCH(x) x "CONFIG_FEATURE_REVERSE_SEARCH"
#else
# define IF_FEATURE_REVERSE_SEARCH(x) x
#endif
#define IF_NOT_FEATURE_REVERSE_SEARCH(x)
#define CONFIG_FEATURE_TAB_COMPLETION 1
#define ENABLE_FEATURE_TAB_COMPLETION 1
#ifdef MAKE_SUID
# define IF_FEATURE_TAB_COMPLETION(x) x "CONFIG_FEATURE_TAB_COMPLETION"
#else
# define IF_FEATURE_TAB_COMPLETION(x) x
#endif
#define IF_NOT_FEATURE_TAB_COMPLETION(x)
#undef CONFIG_FEATURE_USERNAME_COMPLETION
#define ENABLE_FEATURE_USERNAME_COMPLETION 0
#define IF_FEATURE_USERNAME_COMPLETION(x)
#define IF_NOT_FEATURE_USERNAME_COMPLETION(x) x
#define CONFIG_FEATURE_EDITING_FANCY_PROMPT 1
#define ENABLE_FEATURE_EDITING_FANCY_PROMPT 1
#ifdef MAKE_SUID
# define IF_FEATURE_EDITING_FANCY_PROMPT(x) x "CONFIG_FEATURE_EDITING_FANCY_PROMPT"
#else
# define IF_FEATURE_EDITING_FANCY_PROMPT(x) x
#endif
#define IF_NOT_FEATURE_EDITING_FANCY_PROMPT(x)
#undef CONFIG_FEATURE_EDITING_ASK_TERMINAL
#define ENABLE_FEATURE_EDITING_ASK_TERMINAL 0
#define IF_FEATURE_EDITING_ASK_TERMINAL(x)
#define IF_NOT_FEATURE_EDITING_ASK_TERMINAL(x) x
#define CONFIG_FEATURE_NON_POSIX_CP 1
#define ENABLE_FEATURE_NON_POSIX_CP 1
#ifdef MAKE_SUID
# define IF_FEATURE_NON_POSIX_CP(x) x "CONFIG_FEATURE_NON_POSIX_CP"
#else
# define IF_FEATURE_NON_POSIX_CP(x) x
#endif
#define IF_NOT_FEATURE_NON_POSIX_CP(x)
#undef CONFIG_FEATURE_VERBOSE_CP_MESSAGE
#define ENABLE_FEATURE_VERBOSE_CP_MESSAGE 0
#define IF_FEATURE_VERBOSE_CP_MESSAGE(x)
#define IF_NOT_FEATURE_VERBOSE_CP_MESSAGE(x) x
#define CONFIG_FEATURE_COPYBUF_KB 4
#define ENABLE_FEATURE_COPYBUF_KB 1
#ifdef MAKE_SUID
# define IF_FEATURE_COPYBUF_KB(x) x "CONFIG_FEATURE_COPYBUF_KB"
#else
# define IF_FEATURE_COPYBUF_KB(x) x
#endif
#define IF_NOT_FEATURE_COPYBUF_KB(x)
#define CONFIG_FEATURE_SKIP_ROOTFS 1
#define ENABLE_FEATURE_SKIP_ROOTFS 1
#ifdef MAKE_SUID
# define IF_FEATURE_SKIP_ROOTFS(x) x "CONFIG_FEATURE_SKIP_ROOTFS"
#else
# define IF_FEATURE_SKIP_ROOTFS(x) x
#endif
#define IF_NOT_FEATURE_SKIP_ROOTFS(x)
#undef CONFIG_MONOTONIC_SYSCALL
#define ENABLE_MONOTONIC_SYSCALL 0
#define IF_MONOTONIC_SYSCALL(x)
#define IF_NOT_MONOTONIC_SYSCALL(x) x
#define CONFIG_IOCTL_HEX2STR_ERROR 1
#define ENABLE_IOCTL_HEX2STR_ERROR 1
#ifdef MAKE_SUID
# define IF_IOCTL_HEX2STR_ERROR(x) x "CONFIG_IOCTL_HEX2STR_ERROR"
#else
# define IF_IOCTL_HEX2STR_ERROR(x) x
#endif
#define IF_NOT_IOCTL_HEX2STR_ERROR(x)
#define CONFIG_FEATURE_HWIB 1
#define ENABLE_FEATURE_HWIB 1
#ifdef MAKE_SUID
# define IF_FEATURE_HWIB(x) x "CONFIG_FEATURE_HWIB"
#else
# define IF_FEATURE_HWIB(x) x
#endif
#define IF_NOT_FEATURE_HWIB(x)

/*
 * Applets
 */

/*
 * Coreutils
 */

#undef CONFIG_TEST
#define ENABLE_TEST 0
#define IF_TEST(x)
#define IF_NOT_TEST(x) x
#undef CONFIG_FEATURE_TEST_64
#define ENABLE_FEATURE_TEST_64 0
#define IF_FEATURE_TEST_64(x)
#define IF_NOT_FEATURE_TEST_64(x) x

#undef CONFIG_CHOWN
#define ENABLE_CHOWN 0
#define IF_CHOWN(x)
#define IF_NOT_CHOWN(x) x

#undef CONFIG_ECHO
#define ENABLE_ECHO 0
#define IF_ECHO(x)
#define IF_NOT_ECHO(x) x
#undef CONFIG_FEATURE_FANCY_ECHO
#define ENABLE_FEATURE_FANCY_ECHO 0
#define IF_FEATURE_FANCY_ECHO(x)
#define IF_NOT_FEATURE_FANCY_ECHO(x) x

#undef CONFIG_LS
#define ENABLE_LS 0
#define IF_LS(x)
#define IF_NOT_LS(x) x

#undef CONFIG_PRINTF
#define ENABLE_PRINTF 0
#define IF_PRINTF(x)
#define IF_NOT_PRINTF(x) x


/*
 * Editors
 */
#undef CONFIG_FEATURE_VI_ASK_TERMINAL
#define ENABLE_FEATURE_VI_ASK_TERMINAL 0
#define IF_FEATURE_VI_ASK_TERMINAL(x)
#define IF_NOT_FEATURE_VI_ASK_TERMINAL(x) x

/*
 * Miscellaneous Utilities
 */
#undef CONFIG_FEATURE_LESS_ASK_TERMINAL
#define ENABLE_FEATURE_LESS_ASK_TERMINAL 0
#define IF_FEATURE_LESS_ASK_TERMINAL(x)
#define IF_NOT_FEATURE_LESS_ASK_TERMINAL(x) x

#undef CONFIG_FEATURE_CROND_D
#define ENABLE_FEATURE_CROND_D 0
#define IF_FEATURE_CROND_D(x)
#define IF_NOT_FEATURE_CROND_D(x) x

/*
 * Process Utilities
 */
#undef CONFIG_IOSTAT
#define ENABLE_IOSTAT 0
#define IF_IOSTAT(x)
#define IF_NOT_IOSTAT(x) x
#undef CONFIG_LSOF
#define ENABLE_LSOF 0
#define IF_LSOF(x)
#define IF_NOT_LSOF(x) x
#undef CONFIG_MPSTAT
#define ENABLE_MPSTAT 0
#define IF_MPSTAT(x)
#define IF_NOT_MPSTAT(x) x
#undef CONFIG_NMETER
#define ENABLE_NMETER 0
#define IF_NMETER(x)
#define IF_NOT_NMETER(x) x
#undef CONFIG_PMAP
#define ENABLE_PMAP 0
#define IF_PMAP(x)
#define IF_NOT_PMAP(x) x
#undef CONFIG_POWERTOP
#define ENABLE_POWERTOP 0
#define IF_POWERTOP(x)
#define IF_NOT_POWERTOP(x) x
#undef CONFIG_PSTREE
#define ENABLE_PSTREE 0
#define IF_PSTREE(x)
#define IF_NOT_PSTREE(x) x
#undef CONFIG_PWDX
#define ENABLE_PWDX 0
#define IF_PWDX(x)
#define IF_NOT_PWDX(x) x
#undef CONFIG_SMEMCAP
#define ENABLE_SMEMCAP 0
#define IF_SMEMCAP(x)
#define IF_NOT_SMEMCAP(x) x
#undef CONFIG_TOP
#define ENABLE_TOP 0
#define IF_TOP(x)
#define IF_NOT_TOP(x) x
#undef CONFIG_FEATURE_TOP_CPU_USAGE_PERCENTAGE
#define ENABLE_FEATURE_TOP_CPU_USAGE_PERCENTAGE 0
#define IF_FEATURE_TOP_CPU_USAGE_PERCENTAGE(x)
#define IF_NOT_FEATURE_TOP_CPU_USAGE_PERCENTAGE(x) x
#undef CONFIG_FEATURE_TOP_CPU_GLOBAL_PERCENTS
#define ENABLE_FEATURE_TOP_CPU_GLOBAL_PERCENTS 0
#define IF_FEATURE_TOP_CPU_GLOBAL_PERCENTS(x)
#define IF_NOT_FEATURE_TOP_CPU_GLOBAL_PERCENTS(x) x
#undef CONFIG_FEATURE_TOP_SMP_CPU
#define ENABLE_FEATURE_TOP_SMP_CPU 0
#define IF_FEATURE_TOP_SMP_CPU(x)
#define IF_NOT_FEATURE_TOP_SMP_CPU(x) x
#undef CONFIG_FEATURE_TOP_DECIMALS
#define ENABLE_FEATURE_TOP_DECIMALS 0
#define IF_FEATURE_TOP_DECIMALS(x)
#define IF_NOT_FEATURE_TOP_DECIMALS(x) x
#undef CONFIG_FEATURE_TOP_SMP_PROCESS
#define ENABLE_FEATURE_TOP_SMP_PROCESS 0
#define IF_FEATURE_TOP_SMP_PROCESS(x)
#define IF_NOT_FEATURE_TOP_SMP_PROCESS(x) x
#undef CONFIG_FEATURE_TOPMEM
#define ENABLE_FEATURE_TOPMEM 0
#define IF_FEATURE_TOPMEM(x)
#define IF_NOT_FEATURE_TOPMEM(x) x
#undef CONFIG_UPTIME
#define ENABLE_UPTIME 0
#define IF_UPTIME(x)
#define IF_NOT_UPTIME(x) x
#undef CONFIG_FEATURE_UPTIME_UTMP_SUPPORT
#define ENABLE_FEATURE_UPTIME_UTMP_SUPPORT 0
#define IF_FEATURE_UPTIME_UTMP_SUPPORT(x)
#define IF_NOT_FEATURE_UPTIME_UTMP_SUPPORT(x) x
#undef CONFIG_FREE
#define ENABLE_FREE 0
#define IF_FREE(x)
#define IF_NOT_FREE(x) x
#undef CONFIG_FUSER
#define ENABLE_FUSER 0
#define IF_FUSER(x)
#define IF_NOT_FUSER(x) x
#undef CONFIG_KILL
#define ENABLE_KILL 0
#define IF_KILL(x)
#define IF_NOT_KILL(x) x
#undef CONFIG_KILLALL
#define ENABLE_KILLALL 0
#define IF_KILLALL(x)
#define IF_NOT_KILLALL(x) x
#undef CONFIG_KILLALL5
#define ENABLE_KILLALL5 0
#define IF_KILLALL5(x)
#define IF_NOT_KILLALL5(x) x
#undef CONFIG_PGREP
#define ENABLE_PGREP 0
#define IF_PGREP(x)
#define IF_NOT_PGREP(x) x
#undef CONFIG_PIDOF
#define ENABLE_PIDOF 0
#define IF_PIDOF(x)
#define IF_NOT_PIDOF(x) x
#undef CONFIG_FEATURE_PIDOF_SINGLE
#define ENABLE_FEATURE_PIDOF_SINGLE 0
#define IF_FEATURE_PIDOF_SINGLE(x)
#define IF_NOT_FEATURE_PIDOF_SINGLE(x) x
#undef CONFIG_FEATURE_PIDOF_OMIT
#define ENABLE_FEATURE_PIDOF_OMIT 0
#define IF_FEATURE_PIDOF_OMIT(x)
#define IF_NOT_FEATURE_PIDOF_OMIT(x) x
#undef CONFIG_PKILL
#define ENABLE_PKILL 0
#define IF_PKILL(x)
#define IF_NOT_PKILL(x) x
#undef CONFIG_PS
#define ENABLE_PS 0
#define IF_PS(x)
#define IF_NOT_PS(x) x
#undef CONFIG_FEATURE_PS_WIDE
#define ENABLE_FEATURE_PS_WIDE 0
#define IF_FEATURE_PS_WIDE(x)
#define IF_NOT_FEATURE_PS_WIDE(x) x
#undef CONFIG_FEATURE_PS_LONG
#define ENABLE_FEATURE_PS_LONG 0
#define IF_FEATURE_PS_LONG(x)
#define IF_NOT_FEATURE_PS_LONG(x) x
#undef CONFIG_FEATURE_PS_TIME
#define ENABLE_FEATURE_PS_TIME 0
#define IF_FEATURE_PS_TIME(x)
#define IF_NOT_FEATURE_PS_TIME(x) x
#undef CONFIG_FEATURE_PS_ADDITIONAL_COLUMNS
#define ENABLE_FEATURE_PS_ADDITIONAL_COLUMNS 0
#define IF_FEATURE_PS_ADDITIONAL_COLUMNS(x)
#define IF_NOT_FEATURE_PS_ADDITIONAL_COLUMNS(x) x
#undef CONFIG_FEATURE_PS_UNUSUAL_SYSTEMS
#define ENABLE_FEATURE_PS_UNUSUAL_SYSTEMS 0
#define IF_FEATURE_PS_UNUSUAL_SYSTEMS(x)
#define IF_NOT_FEATURE_PS_UNUSUAL_SYSTEMS(x) x
#undef CONFIG_RENICE
#define ENABLE_RENICE 0
#define IF_RENICE(x)
#define IF_NOT_RENICE(x) x
#undef CONFIG_BB_SYSCTL
#define ENABLE_BB_SYSCTL 0
#define IF_BB_SYSCTL(x)
#define IF_NOT_BB_SYSCTL(x) x
#undef CONFIG_FEATURE_SHOW_THREADS
#define ENABLE_FEATURE_SHOW_THREADS 0
#define IF_FEATURE_SHOW_THREADS(x)
#define IF_NOT_FEATURE_SHOW_THREADS(x) x
#undef CONFIG_WATCH
#define ENABLE_WATCH 0
#define IF_WATCH(x)
#define IF_NOT_WATCH(x) x

/*
 * Runit Utilities
 */
#undef CONFIG_SESTATUS
#define ENABLE_SESTATUS 0
#define IF_SESTATUS(x)
#define IF_NOT_SESTATUS(x) x

/*
 * Shells
 */
#ifdef MAKE_SUID
# define IF_CTTYHACK(x) x "CONFIG_CTTYHACK"
#else
# define IF_CTTYHACK(x) x
#endif
#define IF_NOT_CTTYHACK(x)
#define CONFIG_HUSH 1
#define ENABLE_HUSH 1
#ifdef MAKE_SUID
# define IF_HUSH(x) x "CONFIG_HUSH"
#else
# define IF_HUSH(x) x
#endif
#define IF_NOT_HUSH(x)
#define CONFIG_HUSH_BASH_COMPAT 1
#define ENABLE_HUSH_BASH_COMPAT 1
#ifdef MAKE_SUID
# define IF_HUSH_BASH_COMPAT(x) x "CONFIG_HUSH_BASH_COMPAT"
#else
# define IF_HUSH_BASH_COMPAT(x) x
#endif
#define IF_NOT_HUSH_BASH_COMPAT(x)
#define CONFIG_HUSH_BRACE_EXPANSION 1
#define ENABLE_HUSH_BRACE_EXPANSION 1
#ifdef MAKE_SUID
# define IF_HUSH_BRACE_EXPANSION(x) x "CONFIG_HUSH_BRACE_EXPANSION"
#else
# define IF_HUSH_BRACE_EXPANSION(x) x
#endif
#define IF_NOT_HUSH_BRACE_EXPANSION(x)
#define CONFIG_HUSH_HELP 1
#define ENABLE_HUSH_HELP 1
#ifdef MAKE_SUID
# define IF_HUSH_HELP(x) x "CONFIG_HUSH_HELP"
#else
# define IF_HUSH_HELP(x) x
#endif
#define IF_NOT_HUSH_HELP(x)
#define CONFIG_HUSH_INTERACTIVE 1
#define ENABLE_HUSH_INTERACTIVE 1
#ifdef MAKE_SUID
# define IF_HUSH_INTERACTIVE(x) x "CONFIG_HUSH_INTERACTIVE"
#else
# define IF_HUSH_INTERACTIVE(x) x
#endif
#define IF_NOT_HUSH_INTERACTIVE(x)
#define CONFIG_HUSH_SAVEHISTORY 1
#define ENABLE_HUSH_SAVEHISTORY 1
#ifdef MAKE_SUID
# define IF_HUSH_SAVEHISTORY(x) x "CONFIG_HUSH_SAVEHISTORY"
#else
# define IF_HUSH_SAVEHISTORY(x) x
#endif
#define IF_NOT_HUSH_SAVEHISTORY(x)
#define CONFIG_HUSH_JOB 1
#define ENABLE_HUSH_JOB 1
#ifdef MAKE_SUID
# define IF_HUSH_JOB(x) x "CONFIG_HUSH_JOB"
#else
# define IF_HUSH_JOB(x) x
#endif
#define IF_NOT_HUSH_JOB(x)
#define CONFIG_HUSH_TICK 1
#define ENABLE_HUSH_TICK 1
#ifdef MAKE_SUID
# define IF_HUSH_TICK(x) x "CONFIG_HUSH_TICK"
#else
# define IF_HUSH_TICK(x) x
#endif
#define IF_NOT_HUSH_TICK(x)
#define CONFIG_HUSH_IF 1
#define ENABLE_HUSH_IF 1
#ifdef MAKE_SUID
# define IF_HUSH_IF(x) x "CONFIG_HUSH_IF"
#else
# define IF_HUSH_IF(x) x
#endif
#define IF_NOT_HUSH_IF(x)
#define CONFIG_HUSH_LOOPS 1
#define ENABLE_HUSH_LOOPS 1
#ifdef MAKE_SUID
# define IF_HUSH_LOOPS(x) x "CONFIG_HUSH_LOOPS"
#else
# define IF_HUSH_LOOPS(x) x
#endif
#define IF_NOT_HUSH_LOOPS(x)
#define CONFIG_HUSH_CASE 1
#define ENABLE_HUSH_CASE 1
#ifdef MAKE_SUID
# define IF_HUSH_CASE(x) x "CONFIG_HUSH_CASE"
#else
# define IF_HUSH_CASE(x) x
#endif
#define IF_NOT_HUSH_CASE(x)
#define CONFIG_HUSH_FUNCTIONS 1
#define ENABLE_HUSH_FUNCTIONS 1
#ifdef MAKE_SUID
# define IF_HUSH_FUNCTIONS(x) x "CONFIG_HUSH_FUNCTIONS"
#else
# define IF_HUSH_FUNCTIONS(x) x
#endif
#define IF_NOT_HUSH_FUNCTIONS(x)
#define CONFIG_HUSH_LOCAL 1
#define ENABLE_HUSH_LOCAL 1
#ifdef MAKE_SUID
# define IF_HUSH_LOCAL(x) x "CONFIG_HUSH_LOCAL"
#else
# define IF_HUSH_LOCAL(x) x
#endif
#define IF_NOT_HUSH_LOCAL(x)
#define CONFIG_HUSH_RANDOM_SUPPORT 1
#define ENABLE_HUSH_RANDOM_SUPPORT 1
#ifdef MAKE_SUID
# define IF_HUSH_RANDOM_SUPPORT(x) x "CONFIG_HUSH_RANDOM_SUPPORT"
#else
# define IF_HUSH_RANDOM_SUPPORT(x) x
#endif
#define IF_NOT_HUSH_RANDOM_SUPPORT(x)
#define CONFIG_HUSH_EXPORT_N 1
#define ENABLE_HUSH_EXPORT_N 1
#ifdef MAKE_SUID
# define IF_HUSH_EXPORT_N(x) x "CONFIG_HUSH_EXPORT_N"
#else
# define IF_HUSH_EXPORT_N(x) x
#endif
#define IF_NOT_HUSH_EXPORT_N(x)
#define CONFIG_HUSH_MODE_X 1
#define ENABLE_HUSH_MODE_X 1
#ifdef MAKE_SUID
# define IF_HUSH_MODE_X(x) x "CONFIG_HUSH_MODE_X"
#else
# define IF_HUSH_MODE_X(x) x
#endif
#define IF_NOT_HUSH_MODE_X(x)
#undef CONFIG_MSH
#define ENABLE_MSH 0
#define IF_MSH(x)
#define IF_NOT_MSH(x) x
#undef CONFIG_FEATURE_SH_IS_ASH
#define ENABLE_FEATURE_SH_IS_ASH 0
#define IF_FEATURE_SH_IS_ASH(x)
#define IF_NOT_FEATURE_SH_IS_ASH(x) x
#define CONFIG_FEATURE_SH_IS_HUSH 1
#define ENABLE_FEATURE_SH_IS_HUSH 1
#ifdef MAKE_SUID
# define IF_FEATURE_SH_IS_HUSH(x) x "CONFIG_FEATURE_SH_IS_HUSH"
#else
# define IF_FEATURE_SH_IS_HUSH(x) x
#endif
#define IF_NOT_FEATURE_SH_IS_HUSH(x)
#undef CONFIG_FEATURE_SH_IS_NONE
#define ENABLE_FEATURE_SH_IS_NONE 0
#define IF_FEATURE_SH_IS_NONE(x)
#define IF_NOT_FEATURE_SH_IS_NONE(x) x
#undef CONFIG_FEATURE_BASH_IS_ASH
#define ENABLE_FEATURE_BASH_IS_ASH 0
#define IF_FEATURE_BASH_IS_ASH(x)
#define IF_NOT_FEATURE_BASH_IS_ASH(x) x
#undef CONFIG_FEATURE_BASH_IS_HUSH
#define ENABLE_FEATURE_BASH_IS_HUSH 0
#define IF_FEATURE_BASH_IS_HUSH(x)
#define IF_NOT_FEATURE_BASH_IS_HUSH(x) x
#define CONFIG_FEATURE_BASH_IS_NONE 1
#define ENABLE_FEATURE_BASH_IS_NONE 1
#ifdef MAKE_SUID
# define IF_FEATURE_BASH_IS_NONE(x) x "CONFIG_FEATURE_BASH_IS_NONE"
#else
# define IF_FEATURE_BASH_IS_NONE(x) x
#endif
#define IF_NOT_FEATURE_BASH_IS_NONE(x)
#define CONFIG_SH_MATH_SUPPORT 1
#define ENABLE_SH_MATH_SUPPORT 1
#ifdef MAKE_SUID
# define IF_SH_MATH_SUPPORT(x) x "CONFIG_SH_MATH_SUPPORT"
#else
# define IF_SH_MATH_SUPPORT(x) x
#endif
#define IF_NOT_SH_MATH_SUPPORT(x) 
#undef CONFIG_SH_MATH_SUPPORT_64 
#define ENABLE_SH_MATH_SUPPORT_64 0
#ifdef MAKE_SUID
# define IF_SH_MATH_SUPPORT_64(x)
#else
# define IF_SH_MATH_SUPPORT_64(x)
#endif
#define IF_NOT_SH_MATH_SUPPORT_64(x) x
#define CONFIG_FEATURE_SH_EXTRA_QUIET 1
#define ENABLE_FEATURE_SH_EXTRA_QUIET 1
#ifdef MAKE_SUID
# define IF_FEATURE_SH_EXTRA_QUIET(x) x "CONFIG_FEATURE_SH_EXTRA_QUIET"
#else
# define IF_FEATURE_SH_EXTRA_QUIET(x) x
#endif
#define IF_NOT_FEATURE_SH_EXTRA_QUIET(x)
#undef CONFIG_FEATURE_SH_STANDALONE
#define ENABLE_FEATURE_SH_STANDALONE 0
#define IF_FEATURE_SH_STANDALONE(x)
#define IF_NOT_FEATURE_SH_STANDALONE(x) x
#undef CONFIG_FEATURE_SH_NOFORK
#define ENABLE_FEATURE_SH_NOFORK 0
#define IF_FEATURE_SH_NOFORK(x)
#define IF_NOT_FEATURE_SH_NOFORK(x) x
#define CONFIG_FEATURE_SH_HISTFILESIZE 1
#define ENABLE_FEATURE_SH_HISTFILESIZE 1
#ifdef MAKE_SUID
# define IF_FEATURE_SH_HISTFILESIZE(x) x "CONFIG_FEATURE_SH_HISTFILESIZE"
#else
# define IF_FEATURE_SH_HISTFILESIZE(x) x
#endif
#define IF_NOT_FEATURE_SH_HISTFILESIZE(x)


#define ENABLE_FEATURE_VERBOSE 0
