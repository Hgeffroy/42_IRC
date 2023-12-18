# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: twang <twang@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/25 08:29:58 by hgeffroy          #+#    #+#              #
#    Updated: 2023/12/15 10:40:04 by twang            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCES		=	sources/classes/Channel.cpp									\
				sources/classes/Client.cpp									\
				sources/classes/Server.cpp									\
				sources/utils/getIP.cpp										\
				sources/utils/getTime.cpp									\
				sources/utils/printMap.cpp									\
				sources/utils/sendToClient.cpp								\
				sources/cmds/mode_a.cpp										\
				sources/cmds/mode_t.cpp										\
				sources/cmds/motd.cpp										\
				sources/cmds/invite.cpp										\
				sources/cmds/join.cpp										\
				sources/cmds/nick.cpp										\
				sources/cmds/part.cpp										\
				sources/cmds/pass.cpp										\
				sources/cmds/privmsg.cpp									\
				sources/cmds/user.cpp										\
				sources/cmds/who.cpp										\
				sources/cmds/quit.cpp										\
				sources/main.cpp
