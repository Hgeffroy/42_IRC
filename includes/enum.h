/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:34:22 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/07 10:57:36 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

enum	e_type
{
	FD_SERV,		// Client pas encore accepte
	FD_CLIENT,		// Client accepte
	FD_CHANNEL
};

enum	e_cmd
{
	PASS,
	NICK,
	USER,
	PRIVMSG,
	JOIN,
	MODE,
	WHO,
	PART,
	QUIT,
	INVITE,
	TOPIC,
	MOTD,
	PONG,
	LIST,
	KICK,
	AWAY,
	BOT
};

enum	e_mode
{
	i = 'i',
	t = 't',
	k = 'k',
	o = 'o',
	l = 'l',
	b = 'b'
};

#endif