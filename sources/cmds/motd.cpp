/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:59:51 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 09:49:10 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	motd(Server& s, Client& c)
{
	sendToClient(c.getFd(), RPL_MOTDSTART(c.getNick(), s.getName()));
	sendToClient(c.getFd(), RPL_MOTD(c.getNick(), s.getMotd()));
	sendToClient(c.getFd(), RPL_ENDOFMOTD(c.getNick()));
}
