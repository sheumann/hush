SRCS = \
	shell/hush.c \
	shell/match.c \
	shell/math.c \
	shell/random.c \
	shell/shell.common.c \
	shell/glob.c \
	shell/fnmatch.c \
	coreutils/echo.c \
	coreutils/test.c \
	coreutils/test.ptr.hack.c \
	libbb/xfuncs.printf.c \
	libbb/xfuncs.c \
	libbb/xgetcwd.c \
	libbb/getopt32.c \
	libbb/perror.msg.c \
	libbb/xatonum.c \
	libbb/signal.names.c \
	libbb/error.retval.c \
	libbb/xfunc.die.c \
	libbb/safe.strncpy.c \
	libbb/lineedit.c \
	libbb/platform.c \
	libbb/endofname.c \
	libbb/signals.c \
	libbb/skip.whitespc.c \
	libbb/wfopen.c \
	libbb/verror.msg.c \
	libbb/bb.strtonum.c \
	libbb/time.c \
	libbb/printable.str.c \
	libbb/full.write.c \
	libbb/bb.qsort.c \
	libbb/xrealloc.vec.c \
	libbb/read.key.c \
	libbb/unicode.c \
	libbb/safe.write.c \
	libbb/read.c \
	libbb/s.gethostname.c \
	libbb/get.line.c \
	libbb/conc.pathfile.c \
	libbb/last.char.is.c \
	libbb/safe.poll.c \
	libbb/escape.seq.c \
	libbb/cmp.str.array.c \
	libbb/llist.c \
	libbb/parse.mode.c \
	libbb/messages.c \
	libbb/appletlib.c \
	libbb/bb.basename.c \
	libbb/auto.string.c \
	libbb/vfork.and.run.c \
	libbb/poll.c \
	libbb/get.exec.path.c \
	libbb/pgrp.c \
	libbb/qsort.c

HEADERS = \
	include/NUM.APPLETS.h \
	include/appltmetadata.h \
	include/autoconf.h \
	include/busybox.h \
	include/libbb.h \
	include/platform.h \
	include/poll.h \
	include/unicode.h \
	include/xatonum.h \
	shell/glob.h \
	shell/match.h \
	shell/math.h \
	shell/random.h \
	shell/shell.common.h \
	libbb/xatonum.tmplt.c

OBJS = $(SRCS:.c=.o)

INCLUDES = -I include -I shell -I libbb
DEFINES = -Dhush_main=main -DNDEBUG
LIBS = -ltermcap

# CFLAGS suitable for clang or GCC
CFLAGS = -std=c89 -funsigned-char -Wall -Wno-comment -Wno-format-security

PROG = hush

$(PROG): $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $@

$(OBJS): $(HEADERS)
%.o: %.c
	$(CC) $(INCLUDES) $(DEFINES) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(PROG)
