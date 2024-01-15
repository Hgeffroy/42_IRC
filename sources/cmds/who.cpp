/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 08:57:14 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:03:27 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	who(Server& s, Client& c, std::string& str)
{
	size_t 										space = str.find(' ');
	if (space == std::string::npos)
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "WHO", "Need more parameters"));
		return ;
	}
	
	std::string 								mask = str.substr(space + 1, str.length() - (space + 1));
	std::map<std::string, Channel*>				channels = s.getChannels();
	std::map<std::string, Client*>				clients = s.getClients();
	std::map<std::string, Channel*>::iterator	itc = channels.find(mask);

	if (itc != channels.end())
	{
		std::map<std::string, std::string>	chanMembers = itc->second->getMembers();
		for (std::map<std::string, std::string>::iterator it = chanMembers.begin(); it != chanMembers.end(); ++it) {
			std::string flags = "H";
			if (it->second == "~" || it->second == "@")
				flags += "@";
			sendToClient(c.getFd(), RPL_WHOREPLY(c.getNick(), mask, clients[it->first]->getUser(), it->first, clients[it->first]->getUser(), flags));
		}
		sendToClient(c.getFd(), RPL_ENDOFWHO(c.getNick(), mask));
	}
	else
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), mask));
}
