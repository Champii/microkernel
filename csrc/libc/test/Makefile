include ../Makefile.src
include ../Makefile.rules

SRC_LIBC = $(filter-out sys/syscall.c, $(SRC))

SRC_LIBCOMPAT = linux_support.c\
		linux_syscall.S\
		syscall.c\
		dummy_rpc.c

INCLUDES = -I../include/

CFLAGS  += $(INCLUDES)
LDFLAGS += -ereal_start

ifeq ($(RELEASE), 1)
  CFLAGS += $(CFLAGS_RELEASE)
else
  CFLAGS += $(CFLAGS_DEBUG)
endif

SRC_LIBC_PATH 	= ../
BUILD_PATH 	= build/

OBJS_LIBC = $(addprefix $(BUILD_PATH), $(SRC_LIBC:.c=.o))
DIRS_LIBC = $(addprefix $(BUILD_PATH), $(dir $(SRC_LIBC)))
DEPS_LIBC = $(addprefix $(BUILD_PATH), $(SRC_LIBC:.c=.d))

OBJS_LIBCOMPAT = $(addprefix $(BUILD_PATH), $(patsubst %.c, %.o, $(filter %.c, $(SRC_LIBCOMPAT))))
OBJS_LIBCOMPAT += $(addprefix $(BUILD_PATH), $(patsubst %.S, %.o, $(filter %.S, $(SRC_LIBCOMPAT))))
DEPS_LIBCOMPAT = $(addprefix $(BUILD_PATH), $($(filter-out %.S, $(SRC_LIBCOMPAT:.c=.d))))

DEPS_TESTS = $(addprefix $(BUILD_PATH), $(addsuffix .d, $(TESTS)))

DEPS = $(DEPS_LIBC) $(DEPS_LIBCOMPAT) $(DEPS_TESTS)

all: test dummy_serv_pl dummy_serv_pg dummy_serv_io
	./do_test.sh

test: $(BUILD_PATH)test.o libcompat.o mikro_libc_test.a
	$(LD) $(LDFLAGS) $^ -o $@

dummy_serv_pl: $(BUILD_PATH)dummy_serv_pl.o libcompat.o mikro_libc_test.a
	$(LD) $(LDFLAGS) $^ -o $@

dummy_serv_pg: $(BUILD_PATH)dummy_serv_pg.o libcompat.o mikro_libc_test.a
	$(LD) $(LDFLAGS) $^ -o $@

dummy_serv_io: $(BUILD_PATH)dummy_serv_io.o libcompat.o mikro_libc_test.a
	$(LD) $(LDFLAGS) $^ -o $@

libcompat.o: $(OBJS_LIBCOMPAT)
	$(LD) -r $^ -o $@

mikro_libc_test.a: $(OBJS_LIBC) | build_dirs
	$(AR) rs $@ $(OBJS_LIBC)

$(BUILD_PATH)%.o: %.c | build_dirs
	$(CC) $(CFLAGS) -c -MMD $< -o $@

$(BUILD_PATH)%.o: %.S | build_dirs
	$(CC) -E $(INCLUDES) $(ARCH_CFLAGS) $< | $(AS) $(ASFLAGS) -o $@

$(BUILD_PATH)%.o: $(SRC_LIBC_PATH)%.c
	$(CC) $(CFLAGS) -c -MMD $< -o $@

build_dirs:
	mkdir -p $(DIRS_LIBC)

clean:
	rm -rf $(BUILD_PATH)

distclean: clean
	rm -f mikro_libc_test.a
	rm -f libcompat.o
	rm -f test dummy_serv_pl dummy_serv_pg dummy_serv_io

.PHONY: distclean clean

-include $(DEPS)
