/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 13:50:57 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendChannelRPL(int fd, Channel* chan, std::string client, std::string channel, std::string topic, std::string symbol, std::string nickPrefixed)
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string>				members = chan->getMembers();

	if (!chan->getTopic().empty())
		::sendToClient(fd, RPL_TOPIC(client, channel, topic)); // Seulement s'il y a un topic !

	for (it = members.begin(); it != members.end(); ++it)
		::sendToClient(fd, RPL_NAMREPLY(client, symbol, channel, nickPrefixed)); // A changer !!

	::sendToClient(fd, RPL_ENDOFNAMES(client, channel));
}


void	join(Server& s, Client& c, std::string& str)
{
	std::string channelName;
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));
	if (sep2 == -1)
	{
		if (str[sep2] == '\n')
			sep2 = str.size() - 1;
		else
			sep2 = str.size();
	}
	// Mettre des protections !!

	channelName = str.substr(sep1 + 1, sep2 - sep1 - 1);
	if (channelName[0] != '#')
		return ; // Send une erreur ici

	std::string fullMsg = ":" + c.getNick() + " JOIN " + channelName;
	::sendToClient(c.getFd(), fullMsg);

	std::map<std::string, Channel*>			channels = s.getChannels();

	if (channels[channelName]) {
		if (channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
		{
			channels[channelName]->addUser(c);
			sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), channelName, (channels[channelName])->getTopic(), "=", "@randomUser");
			return;
		}
		else {
			std::cerr << "RPL no channel or NO MORE SPACE FOR MORE USER" << std::endl;
			return;
		}
	}
	Channel* newChannel = new Channel(channelName, c.getNick());
	s.addChannel(newChannel);
	sendChannelRPL(c.getFd(), newChannel, c.getNick(), channelName, newChannel->getTopic(), "=", "@randomUser");
}