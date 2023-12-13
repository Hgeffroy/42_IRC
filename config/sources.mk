# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/25 08:29:58 by hgeffroy          #+#    #+#              #
#    Updated: 2023/12/06 12:24:36 by hgeffroy         ###   ########.fr        #
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
				sources/cmds/join.cpp										\
				sources/cmds/nick.cpp										\
				sources/cmds/pass.cpp										\
				sources/cmds/privmsg.cpp									\
				sources/cmds/user.cpp										\
				sources/cmds/who.cpp										\
				sources/main.cpp
