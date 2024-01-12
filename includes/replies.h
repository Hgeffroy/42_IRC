/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:39:40 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/12 14:23:50 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

# define SWITCH_NICK(prev, next)										std::string(":") + prev + " NICK " + next + ENDLINE
# define JOIN_MSG(client, username, channel)							std::string(":") + client + "!" + username + "@localhost" + " JOIN " + channel + ENDLINE
# define PONG_MSG(token)												std::string(":server ") + "PONG " + token + ENDLINE
# define KICK_MSG(client, channel, usr, comment)						std::string(":") + client + " KICK " + channel + " " + usr + comment + ENDLINE
# define PART_MSG(client, username, channel, reason)					std::string(":") + client + "!" + username + "@localhost" + " PART " + channel + " :" + reason + ENDLINE
# define INVITE_MSG(client, username, invited, channel)					std::string(":") + client + "!" + username + "@localhost" + " INVITE " + invited + " " + channel + ENDLINE
# define MODE_MSG(client, username, str)								std::string(":") + client + "!" + username + "@localhost" + " " + str + ENDLINE
# define PRIV_MSG(client, username, dest, msg)							std::string(":") + client + "!" + username + "@localhost" + " PRIVMSG " + dest + " " + msg + ENDLINE

// 001
# define RPL_WELCOME(client, nickname, username)						std::string(":server ") + "001 " + client + " :Welcome to the " + NETWORKNAME + " Network, " + nickname + "!" + username + "@localhost" + ENDLINE
// 002
# define RPL_YOURHOST(client, servername)								std::string(":server ") + "002 " + client + " :Your host is " + servername + ", running version " + VERSION + ENDLINE
// 003
# define RPL_CREATED(client, time)										std::string(":server ") + "003 " + client + " :This server was created " + time + ENDLINE
// 004
# define RPL_MYINFO(client, servername)									std::string(":server ") + "004 " + client + " " + servername + " " + VERSION + " o " + "tpkilb" + " k,l,b" + ENDLINE
// 005
# define RPL_ISUPPORT(client, nicklen, chanlen)							std::string(":server ") + "005 " + client + " " + "NICKLEN=" + nicklen + " CHANNELEN=" + chanlen + " :are supported by this server" + ENDLINE

// 301
# define RPL_AWAY(client, nick)											std::string(":server ") + "301 " + client + " " + nick + " :is away" + ENDLINE
// 305
# define RPL_UNAWAY(client)												std::string(":server ") + "305 " + client + " :You are no longer marked as being away" + ENDLINE
// 306
# define RPL_NOWAWAY(client)											std::string(":server ") + "306 " + client + " :You have been marked as being away" + ENDLINE

// 315
# define RPL_ENDOFWHO(client, mask)										std::string(":server ") + "315 " + client + " " + mask + " :End of /WHO list" + ENDLINE
// 321
# define RPL_LISTSTART(client)											std::string(":server ") + "321 " + client + " Channel :Users  Name" + ENDLINE
// 322
# define RPL_LIST(client, channel, nb, topic)							std::string(":server ") + "322 " + client + " " + channel + " " + nb + " :" + topic + ENDLINE
// 323
# define RPL_LISTEND(client)											std::string(":server ") + "323 " + client + " :End of /LIST" + ENDLINE
// 324
# define RPL_CHANNELMODEIS(client, channel, modes, arguments)			std::string(":server ") + "324 " + client + " " + channel + " " + modes + " " + arguments + ENDLINE

// 331
# define RPL_NOTOPIC(client, channel)									std::string(":server ") + "331 " + client + " " + channel + " :No topic is set" + ENDLINE
// 332
# define RPL_TOPIC(client, channel, topic)								std::string(":server ") + "332 " + client + " " + channel + " :" + topic + ENDLINE

// 336
# define RPL_INVITELIST(client, channel)								std::string(":server ") + "336 " + client + " " + channel + ENDLINE
// 337
# define RPL_ENDOFINVITELIST(client)									std::string(":server ") + "337 " + client + " :End of /INVITE list" + ENDLINE
// 341
# define RPL_INVITING(client, nick, channel)							std::string(":server ") + "341 " + client + " " + nick + " " + channel + ENDLINE

// 352
# define RPL_WHOREPLY(client, channel, username, nick, realname, flags)	std::string(":server ") + "352 " + client + " " + channel + " " + username + " localhost" + " server " + nick + " " + flags + " :" + "1 " + realname + ENDLINE
// 353 (:channel ?)
# define RPL_NAMREPLY(client, symbol, channel, nickPrefixed)			std::string(":server ") + "353 " + client + " " + symbol + " " + channel + " :" + nickPrefixed + ENDLINE

// 366 (:channel ?)
# define RPL_ENDOFNAMES(channel)										std::string(":server ") + "366 " + channel + " :End of /NAMES list" + ENDLINE

// 372
# define RPL_MOTD(client, motd)											std::string(":server ") + "372 " + client + " :" + motd + ENDLINE
// 375
# define RPL_MOTDSTART(client, server)									std::string(":server ") + "375 " + client + " :- " + server + " Message of the day - " + ENDLINE
// 376
# define RPL_ENDOFMOTD(client)											std::string(":server ") + "376 " + client + " :End of /MOTD command." + ENDLINE

// 381
# define RPL_YOUREOPER(client)											std::string(":server ") + "381 " + client + " :You are now an IRC operator" + ENDLINE

#endif