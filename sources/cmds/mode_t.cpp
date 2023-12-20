/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/20 14:28:54 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static bool			isOperator( Client &c, Channel *channel );
static bool			isBanned( Channel *channel, std::string unbanned );
static std::string	getOption( Client &c, char opt, std::string s );
static std::string	getParam( Client &c, std::string s );

/*----------------------------------------------------------------------------*/

static void	showMap( std::map<std::string, std::string>&	map )
{
	for( std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it )
	{
		std::string name = it->first;
		std::cout << BLUE << name;
		std::string status = it->second;
		std::cout << RED << " " << status << std::endl;
	}
}

//i: Set/remove Invite-only channel
void	i_opt( Client &c, Channel *channel, std::string s )
{
	std::string	option = getOption( c, 'i', s );
	if ( option.empty() )
		return ;

	if ( !isOperator( c, channel ) )
		return ;

	if ( s[0] == '+' )
	{
		if ( channel->getInviteStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setInviteStatus( true );
		std::cout << YELLOW << "MODE " << channel->getName() << " +i";
		std::cout << " : Setting the \"invite only\" mode on " << channel->getName() << END << std::endl;
	}
	else if ( s[0] == '-' )
	{
		if ( !channel->getInviteStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setInviteStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -i";
		std::cout << " : Unset the \"invite only\" mode on " << channel->getName() << END << std::endl;
	}
}

//k: Set/remove the channel key (password)
void	k_opt( Client &c, Channel *channel, std::string s )
{
	if ( !isOperator( c, channel ) )
		return ;

	std::string	option = getOption( c, 'k', s );
	if ( option.empty( ) )
		return ;

	if ( s[0] == '+' )
	{
		std::string	password = getParam( c, s );
		if ( password.empty( ) )
			return ;
		if ( channel->getKeyStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setKeyStatus( true );
		channel->setPassword( password );
		std::cout << YELLOW << "MODE " << channel->getName() << " +k";
		std::cout << " : Setting the \"key protect\" mode on " << channel->getName();
		std::cout << " with key : <" << channel->getPassword() << ">" << END << std::endl;
	}
	else if ( s[0] == '-' )
	{
		if ( !channel->getKeyStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setKeyStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -k";
		std::cout << " : Unset the \"key protect\" mode on " << channel->getName() << END << std::endl;
	}
}


//b: Ban/un-ban client from channel
void	b_opt( Client &c, Channel *channel, std::string s )
{
	std::map< std::string, std::string >	members = channel->getMembers();
	std::vector< std::string >				bannedList = channel->getBannedGuest();
	std::string								option = getOption( c, 'b', s );
	if ( option.empty() )
		return ;

	if ( !isOperator( c, channel ) )
		return ;

	if ( s[0] == '+' )
	{
		std::string	banned = getParam( c, s );
		if ( banned.empty( ) )
			return ;
		std::map<std::string, std::string>::iterator ite = members.find( banned );
		if ( ite != members.end() )
		{
			if ( members[ banned ] == "~" )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban a founder from channel."));
				return ;
			}
			if ( members[ banned ] == "@" )
			{
				if ( members[ c.getNick() ] != "~" )
				{
					sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban an operator from channel."));
					return ;
				}
			}
			channel->setBannedGuest( banned );
			channel->removeUserFromChan( banned );
			channel->removeUserFromGuestList( banned );
			std::cout << YELLOW << "MODE " << channel->getName() << " +b";
			std::cout << " : Setting the \"ban\" mode for : <" << banned << ">" << END << std::endl;
			return ;
		}
		for ( std::vector< std::string >::iterator	itb = bannedList.begin(); itb != bannedList.end(); itb++ )
		{
			if ( *itb == banned )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " already banned from this channel."));
				return ;
			}
		}
		sendToClient( c.getFd(), ERR_NOTONCHANNEL( banned, channel->getName() ) );
		return ;
	}
	else if ( s[0] == '-' )
	{
		std::string	unbanned = getParam( c, s );
		if ( unbanned.empty( ) )
			return ;
		if ( isBanned( channel, unbanned ) )
		{
			channel->removeUserFromBanList( unbanned );
			std::cout << PURPLE << "MODE " << channel->getName() << " -b";
			std::cout << " : Unsetting the \"ban\" mode for : <" << unbanned << ">" << END << std::endl;
			return ;
		}
		sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "-b", unbanned, " was not banned from this channel."));
	}
}

/*---- utils ------------------------------------------------------------------*/

static bool	isOperator( Client &c, Channel *channel )
{
	std::map< std::string, std::string >members = channel->getMembers();
	if ( members[ c.getNick( ) ] != "@" && members[ c.getNick( ) ] != "~" )
	{
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED( c.getNick(), channel->getName() ) );
		return ( false );
	}
	return ( true );
}

static bool	isBanned( Channel *channel, std::string unbanned )
{
	std::vector< std::string >	bannedList = channel->getBannedGuest();

	for ( std::vector< std::string >::iterator	it = bannedList.begin(); it != bannedList.end(); it++ )
	{
		if ( *it == unbanned )
			return ( true );
	}
	return ( false );
}

static std::string	getOption( Client &c, char opt, std::string s )
{
	if ( opt == 'i' )
	{
		std::string	option = s;
		if ( option.size() != 2 )
		{
			sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
			return ( "" );
		}
		return ( option );
	}
	if ( s[0] == '+' )
	{
		std::size_t	first_space = s.find( ' ' );
		if ( first_space != std::string::npos )
		{
			std::string	option = s.substr( 0, first_space );
			if ( option.size() != 2 )
			{
				sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
				return ( "" );
			}
			return ( option );
		}
	}
	if ( opt == 'b' )
	{
		if ( s[0] == '-' )
		{
			std::size_t	first_space = s.find( ' ' );
			if ( first_space != std::string::npos )
			{
				std::string	option = s.substr( 0, first_space );
				if ( option.size() != 2 )
				{
					sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
					return ( "" );
				}
				return ( option );
			}
		}
	}
	else if ( s == "-k" )
		return ( s );
	sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE #lol +k <password>" ) );
	return ( "" );
}

static std::string	getParam( Client &c, std::string s )
{
	std::string	parameter;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
		return ( "" );
	}
	if ( first_space != std::string::npos )
	{
		parameter = s.substr( first_space + 1 );
		if ( parameter.empty() )
		{
			sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "ex: MODE #lol +k <parameter>" ) );
			return ( "" );
		}
	}
	return ( parameter );
}
