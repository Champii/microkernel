include Makefile.src
include Makefile.rules

OBJS	= $(SRC:.c=.o)
DEPS	= $(SRC:.c=.d)

TARGET_LIB = libmikro_libc

INCLUDES = -Iinclude/
CFLAGS += $(CFLAGS_32) $(INCLUDES)

ifeq ($(RELEASE), 1)
  CFLAGS += $(CFLAGS_RELEASE)
else
  CFLAGS += $(CFLAGS_DEBUG)
endif

all: $(TARGET_LIB).a $(TARGET_LIB)_no_start.a $(TARGET_LIB)_no_mman_no_start.a

test:
	make -C test/

$(TARGET_LIB).a: $(OBJS)
	$(AR) rs $@ $?

$(TARGET_LIB)_no_start.a: $(filter-out sys/mman.o, $(OBJS))
	$(AR) rs $@ $?

$(TARGET_LIB)_no_mman_no_start.a: $(filter-out sys/mman.o others/_start.o, $(OBJS))
	$(AR) rs $@ $?

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	make -C test/ clean

distclean: clean
	rm -f $(TARGET_LIB).a
	rm -f $(TARGET_LIB)_no_start.a
	rm -f $(TARGET_LIB)_no_mman_no_start.a
	make -C test/ distclean

.PHONY: distclean clean test

-include $(DEPS)
