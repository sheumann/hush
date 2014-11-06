# The *_SRC variables are used to define segments; see the "%.a" recipe.
# shell/hush.c is divided into segments as specified within the file itself.

MAIN_SRC = shell/hush.c

SHELL_OTHER_SRC = \
	shell/match.c \
	shell/math.c \
	shell/random.c \
	shell/shell.common.c \
	shell/glob.c

COREUTILS_SRC = \
	coreutils/echo.c \
	coreutils/test.c \
	coreutils/test.ptr.hack.c
	
LIBBB_A_SRC = \
	libbb/lineedit.c \
	libbb/lineedptrhack.c \
	libbb/appletlib.c \
	libbb/getopt32.c \
	libbb/error.retval.c \
	libbb/endofname.c \
	libbb/bb.strtonum.c \
	libbb/full.write.c \
	libbb/bb.qsort.c \
	libbb/copyfd.c \
	libbb/get.line.c \
	libbb/conc.pathfile.c \
	libbb/last.char.is.c \
	libbb/cmp.str.array.c \
	libbb/llist.c \
	libbb/escape.seq.c \
	libbb/messages.c \
	libbb/bb.basename.c \
	libbb/mempcpy.c 

LIBBB_B_SRC = \
	libbb/perror.msg.c \
	libbb/signal.names.c \
	libbb/ptrtoglobals.c \
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
	libbb/vfork.and.run.c 
	
SRCS = $(MAIN_SRC) $(SHELL_OTHER_SRC) $(COREUTILS_SRC) $(LIBBB_A_SRC) $(LIBBB_B_SRC)
OBJS = $(SRCS:.c=.a)
ROOT = $(MAIN_SRC:.c=.root)

INCLUDES = -I include -I shell -I libbb
DEFINES = -Dhush_main=main -DNDEBUG

# Hack to effectively disable close_on_exec_on method for now.
# This will cause us to leak file descriptors.  TODO: Fix.
DEFINES += -DF_SETFD=-1 -DFD_CLOEXEC=-1

# For correct handling of varargs methods and fork, we need
# optimize bit 3 set (no stack repair code).
# Optimize bit 6 breaks some standard-compliant varargs code,
# and bits 0, 4, and 5 have known bugs.  Disable for now.
OCC_FLAGS = -i -w -a0 -O8
STACKSIZE = 20480

# Add $(OCC_FLAGS) to CFLAGS on dmake
CFLAGS = $(null, $(OCC_FLAGS))

PROG = hush

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $@

%.a: %.c
	$(CC) $(INCLUDES) $(DEFINES) $(CFLAGS) -c $< -o $@ \
		$(eq,$<,$(MAIN_SRC) -s$(STACKSIZE) -r) \
		$(!eq,$(SHELL_OTHER_SRC:s/$<//),$(SHELL_OTHER_SRC) -SSHELLOTHER) \
		$(!eq,$(COREUTILS_SRC:s/$<//),$(COREUTILS_SRC) -SCOREUTILS_) \
		$(!eq,$(LIBBB_A_SRC:s/$<//),$(LIBBB_A_SRC) -SLIBBB_A___) \
		$(!eq,$(LIBBB_B_SRC:s/$<//),$(LIBBB_B_SRC) -SLIBBB_B___) 

.PHONY: clean
clean:
	$(RM) $(OBJS) $(ROOT) $(PROG)
