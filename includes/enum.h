/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:34:22 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:24:30 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

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
	BOT,
	MODERATE,
	moderate
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