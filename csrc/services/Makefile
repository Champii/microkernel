#################################################
#
# Micro kernel - Services
#
# - Build process
#
# Florian Greiner <florian.greiner@epitech.eu>
#
#################################################


PAGING 	= paging
IO 			= io
PROGRAM = program_loader


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
	@echo -n "\n$(BLUE)[Pagination]$(RESET_COLOR)\n"
	@cd $(PAGING) && make -s && cd ..
	@echo -n "\n$(BLUE)[Io]$(RESET_COLOR)\n"
	@cd $(IO) && make -s && cd ..
	@echo -n "\n$(BLUE)[Program Loader]$(RESET_COLOR)\n"
	@cd $(PROGRAM) && make -s && cd ..


install:
	@cd $(PAGING) && make -s install && cd ..
	@cd $(IO) && make -s install && cd ..
	@cd $(PROGRAM) && make -s install && cd ..

clean:
	@cd $(PAGING) && make -s clean && cd ..
	@cd $(IO) && make -s clean && cd ..
	@cd $(PROGRAM) && make -s clean && cd ..
