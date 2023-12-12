/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/12 08:31:06 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	join(Server& s, Client& c, std::string& str)
{
	std::string channelName;
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections !!
	if (sep2 == -1) {
		channelName = str.substr(sep1 + 1, str.size() - sep1 - 2);
	}
	else
		channelName = str.substr(sep1 + 1, sep2 - sep1 - 1);
	if (channelName[0] != '#')
		; // Send une erreur ici

	std::string fullMsg = ":" + c.getNick() + " JOIN " + channelName;
	::sendToClient(c.getFd(), fullMsg);

	std::vector<Channel*>			channels = s.getChannels();
	std::vector<Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
		{
			(*it)->addUser(c);
			::sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), channelName, (*it)->getTopic()));
			::sendToClient(c.getFd(), RPL_NAMREPLY(c.getNick(), "=", channelName, "@RandomUser")); // A changer !!
			::sendToClient(c.getFd(), RPL_ENDOFNAMES(c.getNick(), channelName));

			return ;
		}
	}
	Channel* newChannel = new Channel(channelName, c.getNick());
	s.addChannel(newChannel);
	::sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), channelName, newChannel->getTopic()));
	::sendToClient(c.getFd(), RPL_NAMREPLY(c.getNick(), "=", channelName, "@RandomUser")); // A changer !!
	::sendToClient(c.getFd(), RPL_ENDOFNAMES(c.getNick(), channelName));
}