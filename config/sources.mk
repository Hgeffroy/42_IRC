# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/25 08:29:58 by hgeffroy          #+#    #+#              #
#    Updated: 2024/01/15 14:27:53 by hgeffroy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCES		=	sources/classes/Channel.cpp									\
				sources/classes/Client.cpp									\
				sources/classes/Server.cpp									\
				sources/utils/getTime.cpp									\
				sources/utils/intostr.cpp									\
				sources/utils/sendToClient.cpp								\
				sources/utils/signals.cpp									\
				sources/utils/displayVector.cpp								\
				sources/cmds/topic.cpp										\
				sources/cmds/kick.cpp										\
				sources/cmds/mode.cpp										\
				sources/cmds/moderate.cpp									\
				sources/cmds/motd.cpp										\
				sources/cmds/bot.cpp										\
				sources/cmds/invite.cpp										\
				sources/cmds/join.cpp										\
				sources/cmds/list.cpp										\
				sources/cmds/nick.cpp										\
				sources/cmds/part.cpp										\
				sources/cmds/pass.cpp										\
				sources/cmds/pong.cpp										\
				sources/cmds/privmsg.cpp									\
				sources/cmds/user.cpp										\
				sources/cmds/who.cpp										\
				sources/cmds/quit.cpp										\
				sources/cmds/away.cpp										\
				sources/main.cpp
