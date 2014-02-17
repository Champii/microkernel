SRC = \
	others/kill.c\
	others/errno.c\
	rpc/io.c\
	rpc/paging.c\
	rpc/program_loader.c\
	rpc/rpc.c\
	stdio/buffer_internal.c\
	stdio/itoa_base.c\
	stdio/printf.c\
	stdlib/exit.c\
	stdlib/malloc.c\
	string/memcpy.c\
	string/memset.c\
	string/port.c\
	string/strlen.c\
	string/strcmp.c\
	string/strncmp.c\
	string/strncpy.c\
	sys/socket.c\
	sys/syscall.c\
	sys/wait.c\
	unistd/exec.c\
	unistd/io_wrapper.c\
	unistd/sleep.c

OBJS	= $(addprefix $(BUILD_PATH), $(SRC:.c=.o))
DEPS	= $(addprefix $(BUILD_PATH), $(SRC:.c=.d))

OBJ_mmap = sys/mman.o
OBJ_start = others/_start.o

TARGET_LIB = libmikro_libc

CC	= gcc
AR	= ar

CFLAGS  = -Iinclude/ -m32 -nostdlib -Wall -Wextra -Werror -nostdinc\
	  -fno-builtin -std=c11 -fno-stack-protector -g -mno-mmx -mno-sse\
	  -mno-sse2 -mno-sse3 -mno-3dnow

# Output colors
RESET_COLOR=\033[0;0m
GREEN=\033[0;32;01m
RED=\033[0;31;01m
YELLOW=\033[0;33;01m
BUILT_COLOR=\033[5;32;01m
BLUE=\033[0;34;01m
MAGENTA=\033[0;35;01m
CYAN=\033[0;36;01m

OK_STRING=$(GREEN)[OK]$(RESET_COLOR)
ERROR_STRING=$(RED)[ERRORS]$(RESET_COLOR)
WARN_STRING=$(YELLOW)[WARNINGS]$(RESET_COLOR)
BUILT_STRING=$(BLUE)[$(BUILT_COLOR)Built !$(BLUE)]$(RESET_COLOR)


all: $(TARGET_LIB).a $(TARGET_LIB)_no_start.a $(TARGET_LIB)_no_mman_no_start.a

$(TARGET_LIB).a: $(OBJS) $(OBJ_mmap) $(OBJ_start)
	@echo -n "$(YELLOW)Ar$(RESET_COLOR):   $@ "
	@if $(AR) rs $@ $? 2> /dev/null; then printf "%*s" `expr 28 - \`echo $@ | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

$(TARGET_LIB)_no_start.a: $(OBJS) $(OBJ_mmap)
	@echo -n "$(YELLOW)Ar$(RESET_COLOR):   $@ "
	@if $(AR) rs $@ $? 2> /dev/null; then printf "%*s" `expr 28 - \`echo $@ | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

$(TARGET_LIB)_no_mman_no_start.a: $(OBJS)
	@echo -n "$(YELLOW)Ar$(RESET_COLOR):   $@ "
	@if $(AR) rs $@ $? 2> /dev/null; then printf "%*s" `expr 28 - \`echo $@ | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

%.o: %.c
	@echo -n "$(RED)Lib$(RESET_COLOR):  $< "
	@if $(CC) $(CFLAGS) -c -MMD $< -o $@; then printf "%*s" `expr 28 - \`echo $< | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

distclean: clean
	@rm -f $(TARGET_LIB).a
	@rm -f $(TARGET_LIB)_no_start.a
	@rm -f $(TARGET_LIB)_no_mman_no_start.a

.PHONY: distclean clean

.SILENT: all clean distclean

-include $(DEPS)