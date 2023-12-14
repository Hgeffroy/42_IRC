/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/14 13:45:28 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendChannelRPL(int fd, Channel* chan, std::string client, std::string username, std::string channel, std::string topic, std::string symbol)
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string>				members = chan->getMembers();

	::sendToClient(fd, JOIN_MSG(client, username, getIP(), channel));

	if (!chan->getTopic().empty())
		::sendToClient(fd, RPL_TOPIC(client, channel, topic)); // Seulement s'il y a un topic !

	for (it = members.begin(); it != members.end(); ++it)
	{
		std::string prefix = it->second;
		if (it->second == "~")
			prefix = "@";
		::sendToClient(fd, RPL_NAMREPLY(client, symbol, channel, prefix + it->first)); // A changer !!
	}

	::sendToClient(fd, RPL_ENDOFNAMES(channel));
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

	std::map<std::string, Channel*>			channels = s.getChannels();

	if (channels[channelName]) {
		std::cout << channels[channelName]->getNbUsers() << " < " << channels[channelName]->getUserLimit() << std::endl;
		if (channels[channelName]->getUserLimit() == -1 || channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
		{
			channels[channelName]->addUser(c);
			sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), c.getUser(), channelName, (channels[channelName])->getTopic(), "=");
			return;
		}
		else {
			::sendToClient(c.getFd(), ERR_CHANNELISFULL(c.getNick(), channelName));
			return;
		}
	}
	Channel* newChannel = new Channel(channelName, c.getNick()); // Verifier la taille de channelname
	s.addChannel(newChannel);
	sendChannelRPL(c.getFd(), newChannel, c.getNick(), c.getUser(), channelName, newChannel->getTopic(), "=");
}