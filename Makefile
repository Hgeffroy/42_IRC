# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: twang <twang@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/12 12:52:33 by hgeffroy          #+#    #+#              #
#    Updated: 2024/01/12 10:19:26 by twang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include config/print.mk
include config/sources.mk
include config/includes.mk

.SILENT:

#--variables-------------------------------------------------------------------#

NAME			=		ircserv
DEBUG			=		no
VALGRIND		=		no
HGEFFROY		=		\e]8;;https://profile.intra.42.fr/users/hgeffroy\a\e[34mhgeffroy\e[34m\e]8;;\a
AASCEDU			=		\e]8;;https://profile.intra.42.fr/users/aascedu\a\e[34maascedu\e[34m\e]8;;\a
TWANG			=		\e]8;;https://profile.intra.42.fr/users/twang\a\e[34mtwang\e[34m\e]8;;\a

#--includes & libraries--------------------------------------------------------#

INC_DIR			=		includes/

SRC_DIR			=		sources/

BOT_DIR			=		bot/

#--sources & objects-----------------------------------------------------------#

OBJ_DIR			=		.objs/

#--flags-----------------------------------------------------------------------#

CFLAGS			=		-Wall -Wextra -g3 -std=c++98 -I $(INC_DIR) -MMD -MP #-Werror
CPP				=		c++

#--debug flags-----------------------------------------------------------------#

DFLAGS			=		-fsanitize=address -g3

ifeq ($(DEBUG), yes)
CFLAGS			+=		$(DFLAGS)
endif

#--leaks flags-----------------------------------------------------------------#

LEAKS			=		valgrind --leak-check=full --show-leak-kinds=all --quiet

#--objects---------------------------------------------------------------------#

OBJECTS			=		$(addprefix $(OBJ_DIR), $(SOURCES:.cpp=.o))
DEPS			=		$(addprefix $(OBJ_DIR), $(SOURCES:.cpp=.d))


#--global rules----------------------------------------------------------------#

.DEFAULT_GOAL	=		all

#--header dependancies check---------------------------------------------------#

-include $(DEPS)

#--compilation rules-----------------------------------------------------------#

all:
		$(MAKE) header
		$(MAKE) $(NAME) -j
		./ircserv 2000 popopo

$(NAME): $(OBJECTS)
		$(CPP) $(CFLAGS) $^ -o $@
		$(PRINT_CREATING)

$(OBJ_DIR)%.o: %.cpp
		mkdir -p $(dir $@)
		$(CPP) $(CFLAGS) -c $< -o $@
		$(PRINT_COMPILING)

#--libs, debugs & bonus--------------------------------------------------------#

debug:
		$(MAKE) re -j DEBUG=yes

leaks:
		clear
		$(MAKE) -j VALGRIND=yes
		$(LEAKS) ./ircserv 2000 popopo

bonus:
		$(MAKE) -C $(BOT_DIR)
		./bot/bot 2000 popopo $$KEY

#--print header----------------------------------------------------------------#

header:
	printf "\n${PURPLE}project:\t${END}${BLUE}ft_irc${END}\n"
	printf "${PURPLE}author:\t\t${END}${BLUE}${HGEFFROY}, ${AASCEDU} & ${TWANG}${END}\n"
	printf "${PURPLE}leaks mode:\t${END}${BLUE}${VALGRIND}${END}\n"
	printf "${PURPLE}debug mode:\t${END}${BLUE}${DEBUG}${END}\n"
	printf "${PURPLE}compiler:\t${END}${BLUE}${CPP}${END}\n"
	printf "${PURPLE}flags:\t\t${END}${BLUE}${CFLAGS}${END}\n"
	printf "${PURPLE}date:\t\t${END}${BLUE}2023/11/25${END}\n"
	$(LINE)

#--re, clean & fclean----------------------------------------------------------#

re:
#clear
		$(MAKE) fclean
		$(MAKE) -j all

clean:
		$(MAKE) -C $(BOT_DIR) clean
		$(LINE)
		$(RM) -rf $(OBJ_DIR)
		$(PRINT_CLEAN)

fclean:
#clear
		$(MAKE) -C $(BOT_DIR) fclean
		$(MAKE) clean
		$(RM) $(NAME)
		$(PRINT_FCLEAN)

#--PHONY-----------------------------------------------------------------------#

.PHONY: all debug leaks bonus header re clean fclean
