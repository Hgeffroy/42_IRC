/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:39:40 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/08 12:41:35 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

// 001
# define RPL_WELCOME(client, nickname, username, IP)			std::string(":server ") + "001 " + client + " :Welcome to the " + NETWORKNAME + " Network, " + nickname + "[!" + username + "@" + IP + "]" + ENDLINE
// 002
# define RPL_YOURHOST(client, servername)						std::string(":server ") + "002 " + client + " :Your host is " + servername + ", running version " + VERSION + ENDLINE
// 003
# define RPL_CREATED(client, time)								std::string(":server ") + "003 " + client + " :This server was created " + time + ENDLINE

// 301
# define RPL_AWAY(client, nick)									std::string(":server ") + "301 " + client + " " + nick + " :is away" + ENDLINE

// 332
# define RPL_TOPIC(client, channel, topic)						std::string(":server ") + "332 " + client + " " + channel + " :" + topic + ENDLINE
// 353
# define RPL_NAMREPLY(client, symbol, channel, nickPrefixed)	std::string(":server ") + "353 " + client + " " + symbol + " " + channel + " :" + nickPrefixed + "{" + nickPrefixed + "}" + ENDLINE
// 366
# define RPL_ENDOFNAMES(client, channel)						std::string(":server ") + "366 " + client + " " + channel + " :End of /NAMES list"


#endif