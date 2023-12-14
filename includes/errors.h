/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 12:44:00 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/07 15:00:11 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

// 401
# define ERR_NOSUCHNICK(client, nickname)		std::string(":server ") + "401 " + client + " " + nickname + " :No such nick/channel" + ENDLINE
// 402
# define ERR_NOSUCHSERVER(client, servname)		std::string(":server ") + "402 " + client + " " + servname + " :No such server" + ENDLINE
// 403
# define ERR_NOSUCHCHANNEL(client, channel)		std::string(":server ") + "403 " + client + " " + channel + " :No such channel" + ENDLINE
// 404
# define ERR_CANNOTSENDTOCHAN(client, channel)	std::string(":server ") + "404 " + client + " " + channel + " :Cannot send to channel" + ENDLINE
// 405
# define ERR_TOOMANYCHANNELS(client, channel)	std::string(":server ") + "405 " + client + " " + channel + " :You have joined too many channels" + ENDLINE
// 412
# define ERR_NOTEXTTOSEND(client)				std::string(":server ") + "412 " + client + " " + " :No text to send" + ENDLINE

// 431
# define ERR_NONICKNAMEGIVEN(client)			std::string(":server ") + "431 " + client + " " + " :No nickanme given" + ENDLINE
// 432
# define ERR_ERRONEUSNICKNAME(client, nick)		std::string(":server ") + "432 " + client + " " + nick + " :Erroneus nickname" + ENDLINE
// 433
# define ERR_NICKNAMEINUSE(client, nick)		std::string(":server ") + "433 " + client + " " + nick + " :Nickname is already in use" + ENDLINE

// 441
# define ERR_USERNOTINCHANNEL(client, nick, channel)	std::string(":server ") + "441 " + client + " " + nick + " " + channel + " :They aren't on that channel" + ENDLINE

// 461
# define ERR_NEEDMOREPARAMS(client, command)	std::string(":server ") + "461 " + client + " " + command + " :Not enough parameters" + ENDLINE
// 462
# define ERR_ALREADYREGISTERED(client)			std::string(":server ") + "462 " + client + " :You may not register" + ENDLINE
// 464
# define ERR_PASSWDMISMATCH(client)				std::string(":server ") + "464 " + client + " :Password incorrect" + ENDLINE
// 471
# define ERR_CHANNELISFULL(client, channel)		std::string(":server ") + "471 " + client + " " + channel + " :Cannot join channel (+l)" + ENDLINE
// 472
# define ERR_UNKNOWNMODE(client, modechar)		std::string(":server ") + "472 " + client + " " + modechar + " :is unknown mode char to me" + ENDLINE

#endif