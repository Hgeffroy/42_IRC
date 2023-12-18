/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:59:51 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 14:45:49 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	motd(Server& s, Client& c)
{
	sendToClient(c.getFd(), RPL_MOTDSTART(c.getNick(), s.getName()));
	sendToClient(c.getFd(), RPL_MOTD(c.getNick(), s.getMotd()));
	sendToClient(c.getFd(), RPL_ENDOFMOTD(c.getNick()));
}