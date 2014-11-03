SRCS = \
	shell/hush.c \
	shell/match.c \
	shell/math.c \
	shell/random.c \
	shell/shell.common.c \
	shell/glob.c \
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
	libbb/ptrtoglobals.c \
	libbb/error.retval.c \
	libbb/xfunc.die.c \
	libbb/safe.strncpy.c \
	libbb/lineedit.c \
	libbb/lineedptrhack.c \
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
	libbb/copyfd.c \
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
	libbb/mempcpy.c \
	libbb/vfork.and.run.c \
	libbb/poll.c
OBJS = $(SRCS:.c=.o)
ROOTS = $(SRCS:.c=.root)

INCLUDES = -I include -I shell -I libbb
DEFINES = -Dhush_main=main -DNDEBUG

OCC_FLAGS = -i -w -a0
# Add $(OCC_FLAGS) to CFLAGS on dmake
CFLAGS = $(null, $(OCC_FLAGS))

PROG = hush

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(DEFINES) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS) $(ROOTS) $(PROG)
