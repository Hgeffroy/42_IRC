/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:39:40 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/07 14:38:25 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
# define REPLIES_H

// 001
# define RPL_WELCOME(client, nickname, username, IP)	std::string("001 ") + client + " :Welcome to the " + NETWORKNAME + " Network, " + nickname + "[!" + username + "@" + IP + "]" + ENDLINE
// 002
# define RPL_YOURHOST(client, servername)				std::string("002 ") + client + " :Your host is " + servername + ", running version " + VERSION + ENDLINE
// 003
# define RPL_CREATED(client, time)						std::string("003 ") + client + " :This server was created " + time + ENDLINE

// 301
# define RPL_AWAY(client, nick, msg)					std::string("301 ") + client + " " + nick + " :" + msg + ENDLINE



#endif