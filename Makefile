#################################################
#
# Micro kernel
#
# - Build process
#
# Florian Greiner <florian.greiner@epitech.eu>
#
#################################################

NAME = kernel.bin

NASM = nasm -f aout
CC = gcc
LD = ld
RM = rm -f

PATH_C = csrc
PATH_ASM = asm
PATH_LD = link

CFLAGS = -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include

LD_SRC = $(PATH_LD)/link.ld
ASM_SRC = $(PATH_ASM)/start.asm
C_SRC = $(PATH_C)/main.c

ASM_OBJ = $(ASM_SRC:.asm=.o)
C_OBJ = $(C_SRC:.c=.o)

all: $(NAME)

$(NAME): $(ASM_OBJS) $(C_OBJ)
	$(NASM) -o $(ASM_OBJ) $(ASM_SRC)
	$(LD) -T $(LD_SRC) -o $(NAME) $(ASM_OBJ) $(C_OBJ)

clean:
	$(RM) $(ASM_OBJ) $(C_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

