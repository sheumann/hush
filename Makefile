# The *_SRC variables are used to define segments; see the "%.a" recipe.
# shell/hush.c is divided into segments as specified within the file itself.

MAIN_SRC = shell/hush.c

SHELL_OTHER_SRC = \
	shell/match.c \
	shell/math.c \
	shell/random.c \
	shell/shell.common.c \
	shell/glob.c \
	shell/fnmatch.c

COREUTILS_SRC = \
	coreutils/echo.c \
	coreutils/test.c \
	coreutils/test.ptr.hack.c
	
LIBBB_A_SRC = \
	libbb/lineedit.c 

LIBBB_B_SRC = \
	libbb/appletlib.c \
	libbb/getopt32.c \
	libbb/error.retval.c \
	libbb/endofname.c \
	libbb/bb.strtonum.c \
	libbb/full.write.c \
	libbb/bb.qsort.c \
	libbb/get.line.c \
	libbb/conc.pathfile.c \
	libbb/last.char.is.c \
	libbb/cmp.str.array.c \
	libbb/llist.c \
	libbb/escape.seq.c \
	libbb/messages.c \
	libbb/bb.basename.c \
	libbb/mempcpy.c \
	libbb/get.exec.path.c \
	libbb/exec.gno.c

LIBBB_C_SRC = \
	libbb/perror.msg.c \
	libbb/signal.names.c \
	libbb/safe.strncpy.c \
	libbb/platform.c \
	libbb/signals.c \
	libbb/printable.str.c \
	libbb/read.key.c \
	libbb/safe.write.c \
	libbb/read.c \
	libbb/s.gethostname.c \
	libbb/safe.poll.c \
	libbb/parse.mode.c \
	libbb/poll.c \
	libbb/pgrp.c \
	libbb/qsort.c

LIBBB_D_SRC = \
	libbb/xfuncs.printf.c \
	libbb/xfuncs.c \
	libbb/xgetcwd.c \
	libbb/xatonum.c \
	libbb/xfunc.die.c \
	libbb/skip.whitespc.c \
	libbb/wfopen.c \
	libbb/verror.msg.c \
	libbb/time.c \
	libbb/xrealloc.vec.c \
	libbb/unicode.c \
	libbb/vfork.and.run.c \
	libbb/waitpid.emul.c
	
SRCS = $(MAIN_SRC) $(SHELL_OTHER_SRC) $(COREUTILS_SRC) $(LIBBB_A_SRC) \
	$(LIBBB_B_SRC) $(LIBBB_C_SRC) $(LIBBB_D_SRC)
OBJS = $(SRCS:.c=.a)
ROOT = $(MAIN_SRC:.c=.root)

SHELL_OTHER_SEG = -SSHELLOTHER
COREUTILS_SEG = -SCOREUTILS_
LIBBB_A_SEG = -SLIBBB_A___
LIBBB_C_SEG = -SLIBBB_C___
# We need more separate segments in debug mode because the code is bigger.
.IF $(DEBUG)
LIBBB_B_SEG = -SLIBBB_B___
LIBBB_D_SEG = -SLIBBB_D___
.ELSE
LIBBB_B_SEG = -SLIBBB_A___
LIBBB_D_SEG = -SLIBBB_C___
.END

INCLUDES = -I include -I shell -I libbb
DEFINES = -Dhush_main=main -DNDEBUG

# This should be the ltermcap from GNO 2.0.4. The one from 2.0.6 is broken
# (links to unimplemented functions), so don't use it.
# The 2.0.4 version is in the "lib.shk" file within  
# ftp://ftp.gno.org/pub/apple2/gs.specific/gno/base/v204/gnodisk1.sdk
LIBS = -l/usr/lib/libtermcap.204

# For correct handling of varargs methods and fork, we need
# optimize bit 3 set (no stack repair code).
# Optimize bit 6 breaks some standard-compliant varargs code,
# and bits 0, 4, and 5 have known bugs.  Disable for now.
CFLAGS = -i -w -a0 -O8
STACKSIZE = 8192

.IF $(DEBUG)
CFLAGS += -g -DDEBUG
.END

PROG = hush

$(PROG): $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $@

%.a: %.c
	$(CC) $(INCLUDES) $(DEFINES) $(CFLAGS) -c $< -o $@ \
		$(eq,$<,$(MAIN_SRC) -s$(STACKSIZE) -r) \
		$(!eq,$(SHELL_OTHER_SRC:s/$<//),$(SHELL_OTHER_SRC) $(SHELL_OTHER_SEG)) \
		$(!eq,$(COREUTILS_SRC:s/$<//),$(COREUTILS_SRC) $(COREUTILS_SEG)) \
		$(!eq,$(LIBBB_A_SRC:s/$<//),$(LIBBB_A_SRC) $(LIBBB_A_SEG)) \
		$(!eq,$(LIBBB_B_SRC:s/$<//),$(LIBBB_B_SRC) $(LIBBB_B_SEG)) \
		$(!eq,$(LIBBB_C_SRC:s/$<//),$(LIBBB_C_SRC) $(LIBBB_C_SEG)) \
		$(!eq,$(LIBBB_D_SRC:s/$<//),$(LIBBB_D_SRC) $(LIBBB_D_SEG)) 

.PHONY: clean
clean:
	$(RM) $(OBJS) $(ROOT) $(PROG)
