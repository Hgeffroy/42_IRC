/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:39:40 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 08:52:42 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

// 001
# define RPL_WELCOME(client, nickname, username, IP)									std::string(":server ") + "001 " + client + " :Welcome to the " + NETWORKNAME + " Network, " + nickname + "[!" + username + "@" + IP + "]" + ENDLINE
// 002
# define RPL_YOURHOST(client, servername)												std::string(":server ") + "002 " + client + " :Your host is " + servername + ", running version " + VERSION + ENDLINE
// 003
# define RPL_CREATED(client, time)														std::string(":server ") + "003 " + client + " :This server was created " + time + ENDLINE

// 301
# define RPL_AWAY(client, nick)															std::string(":server ") + "301 " + client + " " + nick + " :is away" + ENDLINE

// 315
# define RPL_ENDOFWHO(client, mask)														std::string(" ") + "315 " + client + " " + mask + " :Emd of WHO list" + ENDLINE
// 332 (:channel ?)
# define RPL_TOPIC(client, channel, topic)												std::string(" ") + "332 " + client + " " + channel + " :" + topic + ENDLINE
// 352
# define RPL_WHOREPLY(client, channel, username, host, nick, flags, hopcount, realname)	std::string(" ") + "352 " + client + " " + channel + " " + username + " " + host + " server " + nick + " " + flags + " :" + hopcount + " " + realname + ENDLINE
// 353 (:channel ?)
# define RPL_NAMREPLY(client, symbol, channel, nickPrefixed)							std::string(" ") + "353 " + client + " " + symbol + " " + channel + " :" + nickPrefixed + "{" + nickPrefixed + "}" + ENDLINE
// 366 (:channel ?)
# define RPL_ENDOFNAMES(client, channel)												std::string(" ") + "366 " + client + " " + channel + " :End of /NAMES list"


#endif