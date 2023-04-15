OUTPUT				=	ircserv_tester

ifeq ($(shell whoami), runner)
	CC				=	@g++ -std=c++11 -fprofile-arcs -ftest-coverage -O0 -fPIC
else
	CC				=	@clang++ -std=c++11
endif
SH					=	@bash
RM					=	@/bin/rm -rf
FLAGS				=	-Wextra -Wall -Werror -g -pthread
LEAKS_FLAGS			=	--tool=memcheck --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes

HEADER_DIR			=	includes
OBJ_DIR				=	obj/
SRC_DIR				=	srcs/
RESULT				=	$(shell cat output.txt)
AUTHOR				=	$(shell cat ../../author)
LAST_COMMIT_DATE	=	$(shell git log -1 --date=format:"%m/%d/%Y" --format="%ad   [%cr]")
LAST_COMMIT_HASH	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format="%H")
LAST_COMMIT_MESSAGE	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format=\'%s\')
OS					=	$(shell uname)
USER				=	$(shell whoami)
ARCH				=	$(shell uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER					=	$(shell lsb_release -sr)
COUNTER				= 	0
COLUMNS				=	$(shell stty size | cut -d" " -f2)

# Mandatory part

SRCS				=

SOURCES				=	main.cpp $(addprefix $(SRC_DIR),$(SRCS))

# Mandatory Objects
OFILE				=	$(SOURCES:%.cpp=%.o)
OBJS				=	$(addprefix $(OBJ_DIR), $(OFILE))

# Dependencies
DEPS				=	$(wildcard $(HEADER_DIR)/*.hpp)

# Functions
disp_indent			=	for I in `seq 1 $(MAKELEVEL)`; do \
						test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
						done

exec_color			=	$(call disp_indent); \
						echo $(1)$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)"\033[31m"; \
						$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)

disp_title			=	$(call disp_indent); \
						echo "$(2)[  $(1) \#$(MAKELEVEL)  ]\033[0m"


################
##   COLORS   ##
################

Y					=	$(shell printf "\033[33m")
R					=	$(shell printf "\033[31m")
G					=	$(shell printf "\033[32m")
CYAN				=	$(shell printf "\033[36m")
B					=	$(shell printf "\033[34m")
X					=	$(shell printf "\033[0m")
UP					=	$(shell printf "\033[A")
CUT					=	$(shell printf "\033[K")
W					=	$(shell printf "\033[37m")
UND					=	$(shell printf "\033[4m")
BLINK				=	$(shell printf "\033[5m")
BOLD				=	$(shell printf "\033[1m")
UP					=	$(shell printf "\033[5A")
NORM_COLOR_T		=	$(shell printf "\033[0;1;141m")
NORM_COLOR			=	$(shell printf "\033[0;38;5;153m")
NORM_COLOR_WAR		=	$(shell printf "\033[38;5;214m")
NORM_COLOR_ERR		=	$(shell printf "\033[0;41m")
BG_X				=	$(shell printf "\033[0;39m")
TAB					=	$(shell printf "\t")

# Make all files.
all:	$(OUTPUT)

run:
	@ TYPE=3 ./ircserv_tester

# Objects directory
$(OBJ_DIR):
			@echo ${CUT}[${Y}$(OUTPUT)]${X} ${B}Creating: ${R}$(OBJ_DIR)${X}
			@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/srcs $(OBJ_DIR)/srcs/utils

# Normal objects
$(OUTPUT): $(OBJ_DIR) $(OBJS)
			@echo
			$(CC) $(OBJS) -I $(HEADER_DIR) -I tests $(FLAGS) -o $(OUTPUT)
			@echo ${B}[-------------------------------------]${X}
			@echo ${B}[---------- ${BOLD}OK${X}${B} - ${BOLD}$(OUTPUT)${X}${B} ----------]${X}
			@echo ${B}[------- 🧟 Let ft_irc 🧟 --------]${X}
			@echo ${B}[-------------------------------------]${X}

$(OBJ_DIR)%.o: %.cpp $(DEPS)
			$(eval COUNTER=$(shell echo $$(($(COUNTER)+1))))
			$(eval SIZE=$(shell wc -c < $<))
			$(eval STR=$(shell echo ${CUT}[${Y}$(OUTPUT)]${X} ${B}Compiling: ${X}$(SIZE) Bytes${R} $<${X}))
			$(eval COMPILED=$(shell echo '$(COUNTER) files compiled'))
			$(eval LEN=$(shell echo $$(($(COLUMNS)-$(shell echo \'$(STR)$(COMPILED)\' | wc -m)))))
			@$(SH) -c "printf '\r$(STR)%$(LEN)s $(COMPILED) $(shell seq -s. $(COUNTER)|tr -d '[:digit:]')\0'"
			$(CC) -c $(FLAGS) -I$(HEADER_DIR) -Itests $< -o $@
weechat:
	@if [ ! $$(docker ps -qa -f name=weechat) ]; then \
    	echo "El contenedor no está en ejecución"; \
	else \
		docker stop weechat; \
		docker rm weechat; \
		docker rmi jkaberg/weechat ; \
	fi
	docker pull jkaberg/weechat
	docker run -it --network=host --name weechat -e UID=1000 -e GID=1000 -v ~/.weechat/config:/weechat  jkaberg/weechat

#-e MYIP=$$(ifconfig en0 | grep 'inet ' | awk '{print $2}')

#/server add ft_irc 10.11.11.1/6667
#/set irc.server.libera.nicks "mynick,mynick2,mynick3,mynick4,mynick5"
#/set irc.server.ft_irc.username "Quines"
#/set irc.server.ft_irc.realname "Marcos"
#/connect ft_irc


##@ Cleaning
clean:		## Clean all objects.
			$(RM) $(OBJ_DIR)
			$(RM) ./tests/res

fclean:		## Remove all objects and executables.
			$(RM) $(OUTPUT)
			$(RM) $(OBJ_DIR)
			$(RM) $(OUTPUT)
			@echo ${CUT}[${Y}$(OUTPUT)]${X} [Garbage collector:${Y} ${OUTPUT} $(OUTPUT) *.o${X}][${G}OK!${X}]

##@ Compilation

re:			## Call fclean => all
			@make clean
			@make all

##@ Testing

cov:		## Make coverage files
			gcov $(OBJS)


rmcov:		## Clean coverage files
			@rm *.gcov coverage*.html coverage*.css

##@ Help
help:		## View all available commands.
			$(shell $(TARGETS_EXE))
			@echo ${CYAN} Proyecto: ${UND}${BOLD}$(OUTPUT)${CYAN}${X}${CYAN}${UND}\\t\\t\\t\\t\\tAuthor: $(AUTHOR)${X}
			@echo ${CYAN} Last commit:${X}
			@echo ${CYAN} \\tDate: $(LAST_COMMIT_DATE)
			@echo ${CYAN} \\tHash: $(LAST_COMMIT_HASH)${X}
			@echo ${CYAN} \\tMessage: $(LAST_COMMIT_MESSAGE)${X}
			@echo ${CYAN}--------------------------------------------------------------------------
			@echo ${CYAN}Available commands: "\\t\\t\\t\\t(${G}Usage:${X} make ${CYAN}<target>${X})"
			@awk 'BEGIN {FS = ":.*##"; printf ""} /^[a-zA-Z_-]+:.*?##/ { printf "  ${CYAN}%-25s${X} %s\n", $$1, $$2 } /^##@/ { printf "\n${UND}${BOLD}%s${X}\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
			@echo ${CYAN}--------------------------------------------------------------------------

.PHONY:		all clean flclean re test
