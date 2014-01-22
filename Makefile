#################################################
#
# Micro kernel
#
# - Build process
#
# Florian Greiner <florian.greiner@epitech.eu>
#
#################################################

# Output file
NAME 					= kernel.bin
DISK_NAME			= disk.img

# Binaries
NASM 					= nasm -f elf32
CC 						= gcc -m32
LD 						= ld -m elf_i386 -z max-page-size=0x1000
RM 						= rm -f

# Paths
PATH_BUILD		= build
PATH_C 				= csrc
PATH_SYSTEM 	= $(PATH_C)/system
PATH_INCLUDE  = $(PATH_C)/include
PATH_SCREEN   = $(PATH_C)/screen
PATH_MM   		= $(PATH_C)/mm
PATH_GDT   		= $(PATH_MM)/gdt
PATH_IDT   		= $(PATH_C)/idt
PATH_ASM 			= asm
PATH_ISRS			= $(PATH_C)/isrs
PATH_LD 			= link

# Cflags
CFLAGS 				= -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./$(PATH_INCLUDE)

# Srcs
LD_SRC 				= $(PATH_LD)/link.ld

ASM_SRC 			= $(PATH_ASM)/start.S 	\
								$(PATH_ASM)/gdt.S 		\
								$(PATH_ASM)/idt.S

C_SRC 				= $(PATH_SYSTEM)/system.c \
								$(PATH_SCREEN)/screen.c	\
								$(PATH_MM)/mm.c 				\
								$(PATH_GDT)/gdt.c 			\
								$(PATH_C)/main.c 				\
								$(PATH_IDT)/idt.c 			\
								$(PATH_ISRS)/isrs.c

# Objs
ASM_OBJ 			= $(ASM_SRC:.S=.o)
C_OBJ 				= $(C_SRC:.c=.o)


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

all: $(NAME)
	@echo "\n$(BUILT_STRING)"

$(NAME): $(ASM_OBJ) $(C_OBJ)
	@echo -n "\n$(BLUE)[Linking]$(RESET_COLOR)"
	@if $(LD) -T $(LD_SRC) -o $(PATH_BUILD)/$(NAME) $(ASM_OBJ) $(C_OBJ); then printf "%25s" " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

clean:
	@echo -n "\n$(BLUE)[Cleaning]$(RESET_COLOR)"
	@if $(RM) $(ASM_OBJ) $(C_OBJ); then printf "%24s" " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

fclean: clean
	@echo -n "\n$(BLUE)[Full Cleaning]$(RESET_COLOR)"
	@if	$(RM) $(PATH_BUILD)/$(NAME); then printf "%19s" " -> ";  echo '$(OK_STRING)\n'; else  echo '$(ERROR_STRING)'; fi


re: fclean all

disk: $(NAME)
	@echo "\n$(BLUE)[Preparing Base Disk]$(RESET_COLOR)"
	cp $(PATH_BUILD)/base/base.img $(PATH_BUILD)/$(DISK_NAME)
	sudo losetup -f $(PATH_BUILD)/$(DISK_NAME)
	sudo losetup -o 1048576 /dev/loop1 /dev/loop0
	sudo mount /dev/loop1 /mnt
	sudo cp -r $(PATH_BUILD)/base/boot /mnt
	sudo cp $(PATH_BUILD)/$(NAME) /mnt/boot
	sudo grub-install --root-directory=/mnt --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop0
	sudo sync
	sudo umount /mnt
	sudo losetup -d /dev/loop1
	sudo losetup -d /dev/loop0

install: $(NAME)
	@echo "\n$(BLUE)[Installing Kernel Into Disk]$(RESET_COLOR)"
	sudo losetup -f build/disk.img
	sudo losetup -o 1048576 /dev/loop1 /dev/loop0
	sudo mount /dev/loop1 /mnt
	sudo cp $(PATH_BUILD)/$(NAME) /mnt/boot
	sudo umount /mnt
	sudo losetup -d /dev/loop1
	sudo losetup -d /dev/loop0

# mountdisk:
# 	sudo losetup -f build/disk.img
# 	sudo losetup -o 1048576 /dev/loop1 /dev/loop0
# 	sudo mount /dev/loop1 /mnt

umountdisk:
	sudo umount /mnt
	sudo losetup -d /dev/loop1
	sudo losetup -d /dev/loop0

%.o: %.S
	@echo -n "$(MAGENTA)Asm$(RESET_COLOR):  $< "
	@if $(NASM) -o $@ $<; then printf "%*s" `expr 28 - \`echo $< | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

%.o : %.c
	@echo -n "$(CYAN)C$(RESET_COLOR):  $< "
	@if $(CC) $(CFLAGS) -c -o $@ $<; then printf "%*s" `expr 30 - \`echo $< | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

