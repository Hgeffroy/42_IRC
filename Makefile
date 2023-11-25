# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/12 12:52:33 by hgeffroy          #+#    #+#              #
#    Updated: 2023/11/25 12:08:12 by hgeffroy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include config/sources.mk
include config/includes.mk

#--variables-------------------------------------------------------------------#

NAME            =       ircserv
DEBUG           =       no

#--includes & libraries--------------------------------------------------------#

INC_DIR         =       includes/

#--sources & objects-----------------------------------------------------------#

OBJ_DIR         =       .objs/

#--flags-----------------------------------------------------------------------#

CFLAGS          =       -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR) -MMD -MP
CPP				=		c++

#--debug flags-----------------------------------------------------------------#

DFLAGS          =       -fsanitize=address -g3

ifeq ($(DEBUG), yes)
CFLAGS          +=      $(DFLAGS)
endif

#--objects---------------------------------------------------------------------#

OBJECTS =		$(addprefix $(OBJ_DIR), $(SOURCES:.cpp=.o))
DEPS	=		$(addprefix $(OBJ_DIR), $(SOURCES:.cpp=.d))

#--global rules----------------------------------------------------------------#

.DEFAULT_GOAL = all

#--header dependancies check---------------------------------------------------#

-include $(DEPS)

#--compilation rules-----------------------------------------------------------#

all:
		$(MAKE) $(NAME) -j

$(NAME): $(OBJECTS)
		$(CPP) $(CFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)%.o: %.cpp
		mkdir -p $(dir $@)
		$(CPP) $(CFLAGS) -c $< -o $@

#--libs, debugs & bonus--------------------------------------------------------#

debug:
		$(MAKE) re -j DEBUG=yes

#--re, clean & fclean----------------------------------------------------------#

re:
		clear
		$(MAKE) fclean
		$(MAKE) -j all

clean:
		$(RM) -rf $(OBJ_DIR)

fclean:
		clear
		$(MAKE) clean
		$(RM) $(NAME)

#--PHONY-----------------------------------------------------------------------#

.PHONY: all debug re clean fclean
