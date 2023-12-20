/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/20 17:06:39 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

// join avec des trucs derriere

static std::string	getChannelName(std::string& str);
static std::string	getChannelPass(std::string& str);
static bool			checkOption_I( Client& c, Channel* channel, std::string channelName );
static bool			checkOption_K( Client& c, Channel* channel, std::string channelPass );
static bool			checkOption_L( Client& c, Channel* channel, std::string channelName );
static bool			checkOption_B( Client& c, Channel* channel, std::string channelName );

void	sendChannelRPL(Server& s, Client& c, Channel* chan)
{
	std::map<std::string, std::string>	members = chan->getMembers();
	std::map<std::string, Client*>		clientList = s.getClients();

	sendToClient(c.getFd(), JOIN_MSG(c.getNick(), c.getUser(), getIP(), chan->getName()));

	if (!chan->getTopic().empty())
		sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), chan->getName(), chan->getTopic())); // Seulement s'il y a un topic !


	// Partie a envoyer a tous les clients du chan
	std::map<std::string, std::string>::iterator	it;
	for (it = members.begin(); it != members.end(); ++it)
	{
		Client* client = clientList[it->first];
		std::map<std::string, std::string>::iterator	it2;
		for (it2 = members.begin(); it2 != members.end(); ++it2)
		{
			std::string prefix = it2->second;
			if (it2->second == "~")
				prefix = "@";
			sendToClient(client->getFd(), RPL_NAMREPLY(client->getNick(), "=", chan->getName(), prefix + it2->first)); // A changer !!
		}
		sendToClient(client->getFd(), RPL_ENDOFNAMES(chan->getName()));
	}
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

	if (channels.find(channelName) != channels.end())
	{
		Channel* channel = channels[channelName];
		if ( !checkOption_I( c, channel, channelName ) )
			return ;
		if ( !checkOption_K( c, channel, channelPass ) )
			return ;
		if ( !checkOption_L( c, channel, channelName ) )
			return ;
		if ( !checkOption_B( c, channel, channelName ) )
			return ;

		std::map<std::string, std::string> members = channel->getMembers();
		if (members.find(c.getNick()) == members.end())
		{
			channel->addUserToChan(c);
			sendChannelRPL(s, c, channel);
		}
		else
			sendToClient(c.getFd(), ERR_USERONCHANNEL(c.getNick(), c.getNick(), channelName));
	}
	else
	{
		Channel* newChannel = new Channel(channelName, c.getNick()); // Verifier la taille de channelname
		s.addChannel(newChannel);
		sendChannelRPL(s, c, newChannel);
	}
}

static bool	checkOption_K( Client& c, Channel* channel, std::string channelPass )
{
	if ( channel->getKeyStatus() )
	{
		if ( channelPass.empty() )
		{
			sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "JOIN"));
			return ( false );
		}
		if ( channelPass != channel->getPassword() )
		{
			sendToClient(c.getFd(), ERR_BADCHANNELKEY(c.getNick(), channel->getName()));
			return ( false );
		}
	}
	return ( true );
}

static bool	checkOption_I( Client& c, Channel* channel, std::string channelName )
{
	std::vector< std::string >	guestList = channel->getGuest();

	if ( channel->getInviteStatus() )
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

static bool	checkOption_L( Client& c, Channel* channel, std::string channelName )
{
	if (channel->getUserLimit() == -1 || channel->getNbUsers() < channel->getUserLimit())
		return ( true );
	else {
		sendToClient(c.getFd(), ERR_CHANNELISFULL(c.getNick(), channelName));
		return ( false );
	}
}

static bool	checkOption_B( Client& c, Channel* channel, std::string channelName )
{
	std::vector<std::string>	bannedList = channel->getBannedGuest();
	for ( std::vector< std::string >::iterator	it = bannedList.begin(); it != bannedList.end() ; it++ )
	{
		if ( *it == c.getNick( ) )
		{
			sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "join", c.getNick( ), " this client has been banned from this channel."));
			return ( false );
		}
	}
	return ( true );
}

static std::string	getChannelName(std::string& str)
{
	std::string	channelName;

	size_t sep1 = str.find(' ');
	size_t sep2 = str.find(' ', sep1 + 1);
	if (sep2 == std::string::npos)
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
