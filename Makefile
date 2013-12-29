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
NASM 					= nasm -f elf
CC 						= gcc
LD 						= ld
RM 						= rm -f

# Paths
PATH_BUILD		= build
PATH_C 				= csrc
PATH_SYSTEM 	= $(PATH_C)/system
PATH_INCLUDE  = $(PATH_C)/include
PATH_SCREEN   = $(PATH_C)/screen
PATH_ASM 			= asm
PATH_LD 			= link

# Cflags
CFLAGS 				= -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./$(PATH_INCLUDE)

# Srcs
LD_SRC 				= $(PATH_LD)/link.ld
ASM_SRC 			= $(PATH_ASM)/start.asm
C_SRC 				= $(PATH_C)/main.c 									\
								$(PATH_SYSTEM)/system.c 					\
								$(PATH_SCREEN)/screen.c

# Objs
ASM_OBJ 			= $(ASM_SRC:.asm=.o)
C_OBJ 				= $(C_SRC:.c=.o)

all: $(NAME)

$(NAME): $(C_OBJ)
	$(NASM) -o $(ASM_OBJ) $(ASM_SRC)
	$(LD) -T $(LD_SRC) -o $(PATH_BUILD)/$(NAME) $(ASM_OBJ) $(C_OBJ)

clean:
	$(RM) $(ASM_OBJ) $(C_OBJ)

fclean: clean
	$(RM) $(PATH_BUILD)/$(NAME)
	$(RM) $(PATH_BUILD)/$(DISK_NAME)

re: fclean all

disk: $(NAME)
	cp $(PATH_BUILD)/base/base.img $(PATH_BUILD)/$(DISK_NAME)
	sudo losetup -f $(PATH_BUILD)/$(DISK_NAME)
	sudo losetup -o 1048576 /dev/loop2 /dev/loop1
	sudo mount /dev/loop2 /mnt
	sudo cp -r $(PATH_BUILD)/base/boot /mnt
	sudo cp $(PATH_BUILD)/$(NAME) /mnt/boot
	sudo grub-install --root-directory=/mnt --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop1
	sudo sync
	sudo umount /mnt
	sudo losetup -d /dev/loop2
	sudo losetup -d /dev/loop1

install: $(NAME)
	sudo losetup -f build/disk.img
	sudo losetup -o 1048576 /dev/loop2 /dev/loop1
	sudo mount /dev/loop2 /mnt
	sudo cp $(PATH_BUILD)/$(NAME) /mnt/boot
	sudo umount /mnt
	sudo losetup -d /dev/loop2
	sudo losetup -d /dev/loop1

mountdisk:
	sudo losetup -f build/disk.img
	sudo losetup -o 1048576 /dev/loop2 /dev/loop1
	sudo mount /dev/loop2 /mnt

umountdisk:
	sudo umount /mnt
	sudo losetup -d /dev/loop2
	sudo losetup -d /dev/loop1
