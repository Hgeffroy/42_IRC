/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:31:06 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 09:55:21 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::map< std::string, std::string >	getChannelMap( Client& c, std::string& s );
static bool									checkOption_I( Client& c, Channel* channel, const std::string& channelName );
static bool									checkOption_K( Client& c, Channel* channel, const std::string& channelPass );
static bool									checkOption_L( Client& c, Channel* channel, const std::string& channelName );
static bool									checkOption_B( Client& c, Channel* channel, const std::string& channelName );
static int									checkChannelName( Client& c, const std::string& channelName );
static void									joinOneChannel( Server& s, Client& c, const std::string& channelName, const std::string& channelPass );
static void									sendChannelRPL( Server& s, Client& c, Channel* chan );

/*----------------------------------------------------------------------------*/

void	join(Server& s, Client& c, std::string& str)
{
	std::map< std::string, std::string >	channelToJoin = getChannelMap(c, str);
	if (channelToJoin.empty())
		return ;

	std::map< std::string, std::string >::iterator it;
	for (it = channelToJoin.begin(); it != channelToJoin.end(); ++it) {
		joinOneChannel(s, c, it->first, it->second);
	}
}

static void	joinOneChannel(Server& s, Client& c, const std::string& channelName, const std::string& channelPass)
{
	if ( checkChannelName( c, channelName ) < 0 )
		return ;

	std::map<std::string, Channel*>	channels = s.getChannels();
	if ( channels.find( channelName ) != channels.end() )
	{
		Channel* channel = channels[channelName];
		if ( c.getNick() != "bot" )
		{
			if ( !checkOption_I( c, channel, channelName ) )
				return ;
			if ( !checkOption_K( c, channel, channelPass ) )
				return ;
			if ( !checkOption_L( c, channel, channelName ) )
				return ;
			if ( !checkOption_B( c, channel, channelName ) )
				return ;
		}

		std::map<std::string, std::string> members = channel->getMembers();
		if (members.find(c.getNick()) == members.end()) {
			sendChannelRPL(s, c, channel);
			channel->addUserToChan(s, c, false);
		}
		else {
			sendToClient( c.getFd(), ERR_USERONCHANNEL( c.getNick(), c.getNick(), channelName ) );
//			return ;
		}
	}
	else {
		Channel* newChannel = new Channel(channelName);
		sendChannelRPL(s, c, newChannel);
		newChannel->addUserToChan(s, c, true);
		s.addChannel(newChannel);
	}
}

static void	sendChannelRPL( Server& s, Client& c, Channel* chan )
{
	std::map<std::string, std::string>	members = chan->getMembers();
	std::map<std::string, Client*>		clientList = s.getClients();

	sendToClient(c.getFd(), JOIN_MSG(c.getNick(), c.getUser(), chan->getName()));

	if (!chan->getTopic().empty())
		sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), chan->getName(), chan->getTopic()));
}

static bool	checkOption_K( Client& c, Channel* channel, const std::string& channelPass )
{
	if ( channel->getKeyStatus() )
	{
		if ( channelPass != channel->getPassword() )
		{
			sendToClient(c.getFd(), ERR_BADCHANNELKEY(c.getNick(), channel->getName()));
			return ( false );
		}
	}
	return ( true );
}

static bool	checkOption_I( Client& c, Channel* channel, const std::string& channelName )
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

static bool	checkOption_L( Client& c, Channel* channel, const std::string& channelName )
{
	if (channel->getUserLimit() == -1 || channel->getNbUsers() < channel->getUserLimit())
		return ( true );
	else {
		sendToClient(c.getFd(), ERR_CHANNELISFULL(c.getNick(), channelName));
		return ( false );
	}
}

static bool	checkOption_B( Client& c, Channel* channel, const std::string& channelName )
{
	std::vector<std::string>	bannedList = channel->getBannedGuest();
	for ( std::vector< std::string >::iterator	it = bannedList.begin(); it != bannedList.end() ; it++ )
	{
		if ( *it == c.getNick( ) )
		{
			sendToClient( c.getFd(), ERR_BANNEDFROMCHAN( c.getNick(), channelName ) );
			return ( false );
		}
	}
	return ( true );
}

static int	checkChannelName( Client& c, const std::string& channelName )
{
	if ( channelName.empty() )
	{
		sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "JOIN #<channel>" ) );
		return ( -1 );
	}
	else if ( ( channelName[0] != '#' && channelName[0] != '&' ) || channelName.size() < 2 )
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channelName));
		return ( -1 );
	}
	std::size_t	comma = channelName.find( ',' );
	if ( comma != std::string::npos )
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channelName));
		return ( -1 );
	}
	if ( channelName == "#bot" && ( c.getUser() != "bot" || c.getNick() != "bot" ) )
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "JOIN", "This channel name is already reserved"));
		return ( -1 );
	}
	if ( channelName.size() > 50 )
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "NICK", "Nickname too long(max size 50)"));
		return ( -1 );
	}
	return ( 0 );
}

static std::map< std::string, std::string >	getChannelMap( Client& c, std::string& s )
{
	std::map< std::string, std::string >	channels;
	std::string								channelNames;
	std::string								passwords;

	std::size_t	first_space = s.find( ' ' );
	if (first_space == std::string::npos) {
		sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "JOIN #<channel>" ) );
		return ( channels );
	}

	std::size_t	second_space = s.find( ' ', first_space + 1 );
	std::string pass;
	std::string chans;
	if ( second_space == std::string::npos )
		chans = s.substr( first_space + 1 );
	else
		chans = s.substr( first_space + 1, second_space - first_space - 1 );
	if ( second_space != std::string::npos )
		pass = s.substr( second_space + 1 );
	std::istringstream	iss(chans);
	std::istringstream	iss2(pass);

	while (std::getline(iss, channelNames, ',')) {
		getline(iss2, passwords, ',');
		channels[channelNames] = passwords;
	}
	return ( channels );

}
