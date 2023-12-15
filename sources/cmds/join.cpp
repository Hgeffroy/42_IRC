/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/15 09:38:53 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static std::string	getChannelName(std::string& str);
static std::string	getChannelPass(std::string& str);
static bool			checkOption_I( Client& c, std::map<std::string, Channel*> channels, \
																std::string channelName );
static bool			checkOption_K( Client& c, std::map<std::string, Channel*> channels, \
										std::string channelName, std::string channelPass );
static bool			checkOption_L( Client& c, std::map<std::string, Channel*>& channels, \
																std::string channelName );

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
	std::map<std::string, Channel*>			channels = s.getChannels();
	std::string								channelName = getChannelName(str);
	std::string								channelPass = getChannelPass(str);

	if ( channelName.empty() )
	{
		std::cerr << RED << "Invalid Channel" << END << std::endl;
		return ;
	}

	if (channels[channelName])
	{
		//faire un show channel stats -> Channel Name : +k -l +i 
		if ( !checkOption_I( c, channels, channelName ) )
			return ;
		if ( !checkOption_K( c, channels, channelName, channelPass ) )
			return ;
		if ( !checkOption_L( c, channels, channelName ) )
			return ;
	}
	else
	{
		Channel* newChannel = new Channel(channelName, c.getNick()); // Verifier la taille de channelname
		s.addChannel(newChannel);
		sendChannelRPL(c.getFd(), newChannel, c.getNick(), c.getUser(), channelName, newChannel->getTopic(), "=");
	}
}

static bool	checkOption_K( Client& c, std::map<std::string, Channel*> channels, std::string channelName, std::string channelPass )
{
	( void )c;
	if ( channels[channelName]->getKeyStatus() )
	{
		if ( channelPass.empty() )
		{
			std::cerr << RED << "No Input for Password" << END << std::endl;
			return ( false );
		}
		if ( channelPass != channels[channelName]->getPassword() )
		{
			std::cerr << RED << "Wrong Password" << END << std::endl;
			return ( false );
		}
	}
	return ( true );
}

static bool	checkOption_I( Client& c, std::map<std::string, Channel*> channels, std::string channelName )
{
	if ( channels[channelName]->getInviteStatus() )
	{
		std::cout << channels[channelName]->getInviteStatus() << std::endl;
		std::cout << c.getInvited() << std::endl;
		if ( c.getInvited() )
		{
			std::cout << BLUE << "This client was invited. NOICE" << END << std::endl;
			return ( true );
		}
		else
		{
			std::cout << RED << "This client was not invited. CONNASSE" << END << std::endl;
			return ( false );
		}
	}
	return ( true );
}

static bool	checkOption_L( Client& c, std::map<std::string, Channel*>& channels, std::string channelName ) // Bizarre d'adduser dans un checkoption...
{
	if (channels[channelName]->getUserLimit() == -1 || channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
	{
		channels[channelName]->addUser(c);
		sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), c.getUser(), channelName, (channels[channelName])->getTopic(), "=");
		return ( true );
	}
	else {
		::sendToClient(c.getFd(), ERR_CHANNELISFULL(c.getNick(), channelName));
		return ( false );
	}
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

	std::size_t	first_space = str.find( ' ' );
	std::size_t	second_space = str.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		channelPass = str.substr( second_space + 1 );
		return ( channelPass );
	}
	return ( "" );
}
