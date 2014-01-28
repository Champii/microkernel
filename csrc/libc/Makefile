SRC = \
	others/kill.c\
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

TARGET_LIB = mikro_libc

CC	= gcc
AR	= ar

CFLAGS  = -Iinclude/ -m32 -nostdlib -Wall -Wextra -Werror -nostdinc\
	  -fno-builtin -std=c11 -fno-stack-protector -g -mno-mmx -mno-sse\
	  -mno-sse2 -mno-sse3 -mno-3dnow

all: $(TARGET_LIB).a $(TARGET_LIB)_no_start.a $(TARGET_LIB)_no_mman_no_start.a

$(TARGET_LIB).a: $(OBJS) $(OBJ_mmap) $(OBJ_start)
	$(AR) rs $@ $?

$(TARGET_LIB)_no_start.a: $(OBJS) $(OBJ_mmap)
	$(AR) rs $@ $?

$(TARGET_LIB)_no_mman_no_start.a: $(OBJS)
	$(AR) rs $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)

distclean: clean
	rm -f $(TARGET_LIB).a
	rm -f $(TARGET_LIB)_no_start.a
	rm -f $(TARGET_LIB)_no_mman_no_start.a

.PHONY: distclean clean

-include $(DEPS)
