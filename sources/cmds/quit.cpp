/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:52:42 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 10:12:17 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* Enlever le client de tous les channels
 * Enlever le client du server */
void	quit(Server& s, Client& c, std::string& str)
{
	std::map<std::string, Channel*>				channels = s.getChannels();
	std::map<std::string, Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		std::map<std::string, std::string>				members = it->second->getMembers();
		std::map<std::string, std::string>::iterator	find = members.find(c.getNick());

		if (find != members.end())
			it->second->removeUserFromChan(find->first);
	}

	s.removeClient(c.getFd());
}