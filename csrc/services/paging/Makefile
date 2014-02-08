#################################################
#
# Micro kernel - Paging service
#
# - Build process
#
# Florian Greiner <florian.greiner@epitech.eu>
#
#################################################

# Output file
NAME 					= paging.mod

# Binaries
CC 						= gcc -m32 -g3
LD 						= ld -m elf_i386 -z max-page-size=0x1000
RM 						= rm -f
MAKE 					= make -s

# Path
PATH_INCLUDE	= ./include
PATH_LIB_INCLUDE	= ../../libc/include

# Cflags
CFLAGS 				= -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./$(PATH_INCLUDE) -I./$(PATH_LIB_INCLUDE)
LDFLAGS				= -L../../libc -lmikro_libc

# Srcs
C_SRC 				= main.c

# Objs
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

$(NAME): $(C_OBJ)
	@echo -n "\n$(BLUE)[Linking]$(RESET_COLOR)"
	@if gcc -o $(NAME) $(C_OBJ) $(LDFLAGS); then printf "%25s" " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

clean:
	@echo -n "\n$(BLUE)[Cleaning]$(RESET_COLOR)"
	@if $(RM) $(C_OBJ); then printf "%24s" " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

fclean: clean
	@echo -n "\n$(BLUE)[Full Cleaning]$(RESET_COLOR)"
	@if	$(RM) $(NAME); then printf "%19s" " -> ";  echo '$(OK_STRING)\n'; else  echo '$(ERROR_STRING)'; fi

re: fclean all

%.o : %.c
	@echo -n "$(CYAN)C$(RESET_COLOR):  $< "
	@if $(CC) $(CFLAGS) -c -o $@ $<; then printf "%*s" `expr 30 - \`echo $< | wc -c\`` " -> "; echo '$(OK_STRING)'; else  echo '$(ERROR_STRING)'; fi

