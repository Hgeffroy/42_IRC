/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:42 by twang             #+#    #+#             */
/*   Updated: 2023/12/20 17:01:39 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::string	getNickName( std::string params );
static std::string	getChannelName( std::string params );

/*----------------------------------------------------------------------------*/

void	invite( Server& s, Client& c, std::string& params )
{
	std::map<std::string, Channel*>			channels = s.getChannels();
	std::string								channel = getChannelName( params );
	std::string								nickname = getNickName( params );

	if ( channel.empty() || nickname.empty() )
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "INVITE"));
		return ;
	}
	else if( channel[0] != '#' )
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channel));
		return ;
	}

	if ( channels[channel] )
	{
		std::map<std::string, std::string>		members = channels[channel]->getMembers();
		if( !channels[channel]->getInviteStatus() )
		{
			std::cerr << PURPLE << "le channel n'est pas en mode +i" << END << std::endl;
			return ;
		}
		std::map<std::string, std::string>::iterator it = members.find(nickname);
		if ( it != members.end() )
		{
			sendToClient(c.getFd(), ERR_USERONCHANNEL(c.getNick(), nickname, channel));
			return ;
		}
		std::map<std::string, std::string>::iterator ite = members.find( c.getNick() );
		if ( ite == members.end() )
		{
			sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), channel));
			return ;
		}
		if ( members[c.getNick()] != "~" && members[c.getNick()] != "@" )
		{
			sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), channel));
			return ;
		}
		std::vector<std::string>					bannedList = channels[channel]->getBannedGuest();
		for ( std::vector< std::string >::iterator	it = bannedList.begin(); it != bannedList.end() ; it++ )
		{
			if ( *it == nickname )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channels[channel]->getName(), "invite", nickname, " this client has been banned from this channel."));
				return ;
			}
		}
		channels[channel]->setGuest( nickname );
		sendToClient(c.getFd(), RPL_INVITING(c.getNick(), nickname, channel));
	}
	else
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channel));
		return ;
	}
}

static std::string	getNickName( std::string params )
{
	std::string	nickname;
	std::size_t	first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		nickname = params.substr( first_space + 1, second_space - first_space - 1 );
		if ( nickname.empty( ) )
			return( "" );
	}
	else
		return( "" );

	return ( nickname );
}

static std::string	getChannelName( std::string params )
{
	std::string	channelname;

	std::size_t	first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	std::size_t third_space = params.find( ' ', second_space + 1 );
	if ( third_space != std::string::npos )
		return ( "invalid" );
	if ( second_space != std::string::npos )
	{
		channelname = params.substr( second_space + 1 );
		if ( channelname.empty( ) )
			return ( "" );
	}
	else
		return( "" );

	return ( channelname );
}

