SRCS = \
	shell/hush.c \
	shell/match.c \
	shell/math.c \
	shell/random.c \
	shell/shell_common.c \
	shell/glob.c \
	coreutils/echo.c \
	coreutils/test.c \
	coreutils/test_ptr_hack.c \
	libbb/xfuncs_printf.c \
	libbb/xfuncs.c \
	libbb/xgetcwd.c \
	libbb/getopt32.c \
	libbb/perror_msg.c \
	libbb/xatonum.c \
	libbb/u_signal_names.c \
	libbb/ptr_to_globals.c \
	libbb/default_error_retval.c \
	libbb/xfunc_die.c \
	libbb/safe_strncpy.c \
	libbb/lineedit.c \
	libbb/lineedit_ptr_hack.c \
	libbb/platform.c \
	libbb/endofname.c \
	libbb/signals.c \
	libbb/skip_whitespace.c \
	libbb/wfopen.c \
	libbb/verror_msg.c \
	libbb/bb_strtonum.c \
	libbb/time.c \
	libbb/printable_string.c \
	libbb/full_write.c \
	libbb/bb_qsort.c \
	libbb/xrealloc_vector.c \
	libbb/copyfd.c \
	libbb/read_key.c \
	libbb/unicode.c \
	libbb/safe_write.c \
	libbb/read.c \
	libbb/safe_gethostname.c \
	libbb/get_line_from_file.c \
	libbb/concat_path_file.c \
	libbb/last_char_is.c \
	libbb/safe_poll.c \
	libbb/process_escape_sequence.c \
	libbb/compare_string_array.c \
	libbb/llist.c \
	libbb/parse_mode.c \
	libbb/messages.c \
	libbb/appletlib.c \
	libbb/get_last_path_component.c \
	libbb/mempcpy.c \
	libbb/vfork_and_run.c
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
