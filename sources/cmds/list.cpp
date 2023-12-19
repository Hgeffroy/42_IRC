/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:44:45 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/19 15:01:35 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	list(Server& s, Client& c, std::string str)
{
	(void)str;
	std::map<std::string, Channel*> channels = s.getChannels();
	std::map<std::string, Channel*>::iterator it;

	sendToClient(c.getFd(), RPL_LISTSTART(c.getNick()));
	for (it = channels.begin(); it != channels.end(); ++it)
		sendToClient(c.getFd(), RPL_LIST(c.getNick(), it->second->getName(), intostr(it->second->getNbUsers()), it->second->getTopic()));
	sendToClient(c.getFd(), RPL_LISTEND(c.getNick()));
}