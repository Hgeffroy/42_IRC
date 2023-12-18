/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:42 by twang             #+#    #+#             */
/*   Updated: 2023/12/18 10:37:38 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static std::string	getNickName( std::string params );
static std::string	getChannelName( std::string params );

void	invite( Server& s, Client& c, std::string& params )
{
	std::map<std::string, Channel*>			channels = s.getChannels();
	std::string								channel = getChannelName( params );
	std::string								nickname = getNickName( params );

	if ( channel.empty() || nickname.empty() )
	{
		/*Servers MAY allow the INVITE with no parameter,
		and reply with a list of channels the sender is invited to as 
		RPL_INVITELIST (336) numerics, ending with a RPL_ENDOFINVITELIST (337) numeric.*/
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "INVITE"));
		// std::cerr << PURPLE << "(461) ERR_NEEDMOREPARAMS" << END << std::endl;
		return ;
	}
	std::map<std::string, std::string> 		members = channels[channel]->getMembers();

	if ( channels[channel] )
	{
		if( !channels[channel]->getInviteStatus() )
		{
			std::cerr << PURPLE << "le channel n'est pas en mode +i" << END << std::endl;
			return ;
		}
		std::map<std::string, std::string>::iterator it = members.find(nickname);
		if ( it != members.end() )
		{
			sendToClient(c.getFd(), ERR_USERONCHANNEL(c.getNick(), nickname, channel));
			// std::cerr << PURPLE << "( 443 ) ERR_USERONCHANNEL" << END << std::endl;
			return ;
		}
		std::map<std::string, std::string>::iterator ite = members.find( c.getNick() );
		if ( ite != members.end() )
		{
			// std::cerr << PURPLE << "( jesaispascombienc ) ERR_NOTONCHANNEL" << END << std::endl;
			sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), channel));
			return ;
		}
		if ( members[c.getNick()] != "~" && members[c.getNick()] != "@" )
		{
			// std::cerr << PURPLE << "( 482 ) ERR_CHANOPRIVSNEEDED" << END << std::endl;
			sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), channel));
			return ;
		}
		// std::cout << PURPLE << c.getNick() << " has invited " << nickname << END << std::endl;
		channels[channel]->setGuest( nickname );
		/*Only members of the channel are allowed to invite other users.
		Otherwise, the server MUST reject the command with the ERR_NOTONCHANNEL numeric.*/
	}
	else
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), channel));
		return ;
	}
	// std::cerr << "( 403 ) ERR_NOSUCHCHANNEL" << std::endl;

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
	{
		std::cout << "TOO MANY PARAMS WESH" << std::endl;
		return ( "" );
	}
	if ( second_space != std::string::npos )
	{
		channelname = params.substr( second_space + 1 );
		if ( channelname.empty( ) || channelname[0] != '#' )
		{
			std::cout << "BAD NAME" << std::endl;
			return( "" );
		}
	}
	else
		return( "" );

	return ( channelname );
}

