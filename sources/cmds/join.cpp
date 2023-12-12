/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/12 09:29:11 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendChannelRPL(int fd, std::string client, std::string channel, std::string topic, std::string symbol, std::string nickPrefixed)
{
	::sendToClient(fd, RPL_TOPIC(client, channel, topic));
	::sendToClient(fd, RPL_NAMREPLY(client, symbol, channel, nickPrefixed)); // A changer !!
	::sendToClient(fd, RPL_ENDOFNAMES(client, channel));
}


void	join(Server& s, Client& c, std::string& str)
{
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections !!

	std::string	channelName = str.substr(sep1 + 1, sep2 - sep1 - 1);
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
			sendChannelRPL(c.getFd(), c.getNick(), channelName, (*it)->getTopic(), "=", "@randomUser");
			return ;
		}
	}
	Channel* newChannel = new Channel(channelName, c.getNick());
	s.addChannel(newChannel);
	sendChannelRPL(c.getFd(), c.getNick(), channelName, newChannel->getTopic(), "=", "@randomUser");

}