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

static std::string	getChannelName(std::string& str);
static std::string	getChannelPass(std::string& str);

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
	std::string	channelName = getChannelName(str);
	std::string	channelPass = getChannelPass(str);

	if ( channelName.empty() )
	{
		std::cerr << RED << "Invalid Channel" << END << std::endl;
		return ;
	}
	if ( channelPass.empty() )
	{
		std::cerr << RED << "Invalid Password" << END << std::endl;
		return ;
	}

	std::string fullMsg = ":" + c.getNick() + " JOIN " + channelName;
	::sendToClient(c.getFd(), fullMsg);

	std::map<std::string, Channel*>			channels = s.getChannels();

	if (channels[channelName]) {
		std::cout << channels[channelName]->getNbUsers() << " < " << channels[channelName]->getUserLimit() << std::endl;
		if (channels[channelName]->getUserLimit() == -1 || channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
		{
			channels[channelName]->addUser(c);
			sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), channelName, (channels[channelName])->getTopic(), "=", "@randomUser");
			return;
		}
		else {
			std::cerr << "RPL NO MORE SPACE FOR MORE USER" << std::endl;
			return;
		}
	}
	Channel* newChannel = new Channel(channelName, c.getNick()); // Verifier la taille de channelname
	s.addChannel(newChannel);
	sendChannelRPL(c.getFd(), newChannel, c.getNick(), channelName, newChannel->getTopic(), "=", "@randomUser");
}

static std::string	getChannelName(std::string& str)
{
	std::string	channelName;

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
		return ( "" ); // Send une erreur ici
	return ( channelName );
}

static std::string	getChannelPass(std::string& str)
{
	std::string	channelPass;

	std::cout << YELLOW << str << END << std::endl;

	std::size_t	first_space = str.find( ' ' );
	std::size_t	second_space = str.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		channelPass = str.substr( second_space + 1 );
		std::cout << YELLOW << "-" << channelPass << "-" << END << std::endl;
		return ( channelPass );
	}
	return ( "" );
}
