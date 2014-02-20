#################################################
#
# Micro kernel - Regular process
#
# - Build process
#
# Florian Greiner <florian.greiner@epitech.eu>
#
#################################################


SHELL_PATH 	= shell

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


all:
	@echo -n "\n$(BLUE)[Shell]$(RESET_COLOR)\n"
	@cd $(SHELL_PATH) && make -s && cd ..

install:
	@cd $(SHELL_PATH) && make -s install && cd ..

clean:
	@cd $(SHELL_PATH) && make -s clean && cd ..
