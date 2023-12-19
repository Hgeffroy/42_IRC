/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/19 12:57:33 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


static std::string	getChannelName(std::string& str);
static std::string	getChannelPass(std::string& str);
static bool			checkOption_I( Client& c, std::map<std::string, Channel*> channels, \
																std::string channelName );
static bool			checkOption_K( Client& c, std::map<std::string, Channel*> channels, \
										std::string channelName, std::string channelPass );
static bool			checkOption_L( Client& c, std::map<std::string, Channel*> channels, \
																std::string channelName );

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

	if ( channelName.empty() )
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channelName));
		return ;
	}

	if (channels[channelName])
	{
		if ( !checkOption_I( c, channels, channelName ) )
			return ;
		if ( !checkOption_K( c, channels, channelName, channelPass ) )
			return ;
		if ( !checkOption_L( c, channels, channelName ) )
			return ;
		channels[channelName]->addUserToChan(c);
		sendChannelRPL(s, c, channels[channelName]);
	}
	else
	{
		Channel* newChannel = new Channel(channelName, c.getNick()); // Verifier la taille de channelname
		s.addChannel(newChannel);
		sendChannelRPL(s, c, newChannel);
	}
}

static bool	checkOption_K( Client& c, std::map<std::string, Channel*> channels, std::string channelName, std::string channelPass )
{
	( void )c;
	if ( channels[channelName]->getKeyStatus() )
	{
		if ( channelPass.empty() )
		{
			std::cerr << PURPLE << "No Input for Password" << END << std::endl;
			return ( false );
		}
		if ( channelPass != channels[channelName]->getPassword() )
		{
			std::cerr << PURPLE << "Wrong Password" << END << std::endl;
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

static bool	checkOption_L( Client& c, std::map<std::string, Channel*> channels, std::string channelName )
{
	if (channels[channelName]->getUserLimit() == -1 || channels[channelName]->getNbUsers() < channels[channelName]->getUserLimit())
	{
//		channels[channelName]->addUserToChan(c);
//		sendChannelRPL(c.getFd(), channels[channelName], c.getNick(), c.getUser(), channelName, (channels[channelName])->getTopic(), "=");
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
