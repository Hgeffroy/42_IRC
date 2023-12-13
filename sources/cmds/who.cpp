/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 08:57:14 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 10:59:37 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	who(Server& s, Client& c, std::string& str)
{
	int 								space = static_cast<int>(str.find(' '));
	std::string 						mask = str.substr(space + 1, str.length() - (space + 1));
	std::map<std::string, Channel*>		channels = s.getChannels();
	std::map<std::string, Client*>		clients = s.getClients();
	Channel* 							chan = channels[mask];
	std::map<std::string, std::string>	chanMembers = chan->getMembers();

	if (mask[0] == '#')
	{
		for (std::map<std::string, std::string>::iterator it = chanMembers.begin(); it != chanMembers.end(); ++it)
			sendToClient(c.getFd(), RPL_WHOREPLY(it->first, mask, clients[it->first]->getUser(), "localhost", it->first, "H" + it->second, '1', clients[it->first]->getUser())); // H if user here, G if gone, @ for ops (second thing on the map in chan)
		// A envoyer pour tous les membres du chan
		// mask of RPL_ENDOFWHO is the mask name
		sendToClient(c.getFd(), RPL_ENDOFWHO(c.getNick(), mask));
	}
}