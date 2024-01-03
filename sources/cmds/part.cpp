/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:05:57 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/03 10:56:09 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	part(Server& s, Client& c, std::string& str) // Need more params
{
	std::size_t	space1 = str.find(' ');
	std::size_t	space2 = str.find(' ', space1 + 1);

	if (space1 == std::string::npos || space2 == std::string::npos)
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PART"));
		return ;
	}

	std::string	chan = str.substr(space1 + 1, space2 - space1 - 1);

	std::map<std::string, Channel*>				channels = s.getChannels();
	std::map<std::string, Channel*>::iterator	itChan = channels.find(chan);

	if (itChan == channels.end())
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), chan));
		return ;
	}

	std::map<std::string, std::string>				members = itChan->second->getMembers();
	std::map<std::string, std::string>::iterator	itMembers = members.find(c.getNick());

	if (itMembers == members.end())
	{
		sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), chan));
		return ;
	}

	itChan->second->removeUserFromChan(c.getNick());
	//nous avons un part qui detruit le client .
}