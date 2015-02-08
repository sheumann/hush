/* vi: set sw=4 ts=4: */
/*
 * Licensed under GPLv2 or later, see file LICENSE in this source tree.
 */
#ifndef BUSYBOX_H
#define BUSYBOX_H 1

#define BB_VER "1.23.1 (GNO hush 1.1)"

#include "libbb.h"
/* BB_DIR_foo and BB_SUID_bar constants: */
#include "appltmetadata.h"

PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN

#if ENABLE_FEATURE_PREFER_APPLETS
# define APPLET_IS_NOFORK(i) (applet_nameofs[i] & (1 << 12))
# define APPLET_IS_NOEXEC(i) (applet_nameofs[i] & (1 << 13))
#else
# define APPLET_IS_NOFORK(i) 0
# define APPLET_IS_NOEXEC(i) 0
#endif

#if ENABLE_FEATURE_SUID
# define APPLET_SUID(i) ((applet_nameofs[i] >> 14) & 0x3)
#endif


/* Length of these names has effect on size of libbusybox
 * and "individual" binaries. Keep them short.
 */
#if ENABLE_BUILD_LIBBUSYBOX
#if ENABLE_FEATURE_SHARED_BUSYBOX
int lbb_main(char **argv) EXTERNALLY_VISIBLE;
#else
int lbb_main(char **argv);
#endif
#endif

POP_SAVED_FUNCTION_VISIBILITY

#endif
