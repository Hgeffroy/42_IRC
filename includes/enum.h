/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 13:34:22 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/29 13:35:08 by hgeffroy         ###   ########.fr       */
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
	JOIN
};

#endif