/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/19 15:11:49 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::string	getChannelName(std::string& str);
static std::string	getChannelPass(std::string& str);
static bool			checkOption_I( Client& c, std::map<std::string, Channel*> channels, \
																std::string channelName );
static bool			checkOption_K( Client& c, std::map<std::string, Channel*> channels, \
										std::string channelName, std::string channelPass );
static bool			checkOption_L( Client& c, std::map<std::string, Channel*> channels, \
																std::string channelName );

/*----------------------------------------------------------------------------*/

void	sendChannelRPL(int fd, Channel* chan, std::string client, std::string username, \
								std::string channel, std::string topic, std::string symbol)
{
	std::map<std::string, std::string>::iterator	it;
	std::map<std::string, std::string>				members = chan->getMembers();

	sendToClient(fd, JOIN_MSG(client, username, getIP(), channel));

	if (!chan->getTopic().empty())
		sendToClient(fd, RPL_TOPIC(client, channel, topic)); // Seulement s'il y a un topic !

	for (it = members.begin(); it != members.end(); ++it)
	{
		std::string prefix = it->second;
		if (it->second == "~")
			prefix = "@";
		sendToClient(fd, RPL_NAMREPLY(client, symbol, channel, prefix + it->first)); // A changer !!
	}

	sendToClient(fd, RPL_ENDOFNAMES(channel));
}


void	join(Server& s, Client& c, std::string& str)
{
	std::map<std::string, Channel*>			channels = s.getChannels();
	std::string								channelName = getChannelName(str);
	std::string								channelPass = getChannelPass(str);

	std::cout << YELLOW << "=" << channelName << "=" << std::endl;
	if ( channelName.empty() )
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "JOIN"));
		return ;
	}
	if ( channelName[0] != '#' )
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channelName));
		return ;
	}

	if ( channels[channelName] )
	{
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
	if ( channels[channelName]->getKeyStatus() )
	{
		if ( channelPass.empty() )
		{
			sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "JOIN"));
			return ( false );
		}
		if ( channelPass != channels[channelName]->getPassword() )
		{
			sendToClient(c.getFd(), ERR_BADCHANNELKEY(c.getNick(), channelName));
			return ( false );
		}
	}
	return ( true );
}

static bool	checkOption_I( Client& c, std::map<std::string, Channel*> channels, std::string channelName )
{
	std::vector< std::string >	guestList = channels[channelName]->getGuest();

	if ( channels[channelName]->getInviteStatus() )
	{
		for ( std::vector<std::string>::iterator it = guestList.begin(); it != guestList.end(); it++ )
		{
			if ( *it == c.getNick() )
				return ( true );
		}
		sendToClient(c.getFd(), ERR_INVITEONLYCHAN(c.getNick(), channelName));
		return ( false );
	}
	return ( true );
}

static bool	checkOption_L( Client& c, std::map<std::string, Channel*> channels, std::string channelName ) // Bizarre d'addUserToChan dans un checkoption...
{
	if (channels[channelName]->getUserLimit() == -1 || channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
	{
		channels[channelName]->addUserToChan(c);
		sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), c.getUser(), channelName, (channels[channelName])->getTopic(), "=");
		return ( true );
	}
	else {
		sendToClient(c.getFd(), ERR_CHANNELISFULL(c.getNick(), channelName));
		return ( false );
	}
}

static std::string	getChannelName(std::string& str)
{
	std::string	channelName;

	std::size_t	sep1 = str.find(' ');
	std::size_t	sep2 = str.find(' ', sep1 + 1);
	if ( sep2 == std::string::npos )
	{
		if (str[sep2] == '\n')
			sep2 = str.size() - 1;
		else
			sep2 = str.size();
	}
	// Mettre des protections !!
	channelName = str.substr(sep1 + 1, sep2 - sep1 - 1);
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
