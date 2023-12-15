/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:39:40 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/14 13:30:44 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

# define JOIN_MSG(client, username, IP, channel)										std::string(":") + client + "!" + username + "@" + IP + " JOIN " + channel + ENDLINE

// 001
# define RPL_WELCOME(client, nickname, username, IP)									std::string(":server ") + "001 " + client + " :Welcome to the " + NETWORKNAME + " Network, " + nickname + "!" + username + "@" + IP + ENDLINE
// 002
# define RPL_YOURHOST(client, servername)												std::string(":server ") + "002 " + client + " :Your host is " + servername + ", running version " + VERSION + ENDLINE
// 003
# define RPL_CREATED(client, time)														std::string(":server ") + "003 " + client + " :This server was created " + time + ENDLINE
// 004
# define RPL_MYINFO(client, servername)													std::string(":server ") + "004 " + client + " " + servername + " " + VERSION + " o " + "tpkil" + " k,l" + ENDLINE
// 005
# define RPL_ISUPPORT(client, nicklen, chanlen)											std::string(":server ") + "005 " + client + " " + "NICKLEN=" + nicklen + " CHANNELEN=" + chanlen + " :are supported by this server" + ENDLINE

// 301
# define RPL_AWAY(client, nick)															std::string(":server ") + "301 " + client + " " + nick + " :is away" + ENDLINE

// 315
# define RPL_ENDOFWHO(client, mask)														std::string(":server ") + "315 " + client + " " + mask + " :End of /WHO list" + ENDLINE
// 332 (:channel ?)
# define RPL_TOPIC(client, channel, topic)												std::string(":server ") + "332 " + client + " " + channel + " :" + topic + ENDLINE
// 352
# define RPL_WHOREPLY(client, channel, username, host, nick, realname)					std::string(":server ") + "352 " + client + " " + nick + " " + channel + " " + username + " " + host + " server " + " :" + realname + ENDLINE
// 353 (:channel ?)
# define RPL_NAMREPLY(client, symbol, channel, nickPrefixed)							std::string(":server ") + "353 " + client + " " + symbol + " " + channel + " :" + nickPrefixed + ENDLINE
// 366 (:channel ?)
# define RPL_ENDOFNAMES(channel)														std::string(":server ") + "366 " + channel + " :End of /NAMES list" + ENDLINE

#endif