/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/19 15:14:26 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static bool			isOperator( Client &c, Channel *channel );
static std::string	getOption( Client &c, char opt, std::string s );
static std::string	getPassword( Client &c, std::string s );

/*----------------------------------------------------------------------------*/

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
		std::string	password = getPassword( c, s );
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

static std::string	getOption( Client &c, char opt, std::string s )
{
	std::cout << YELLOW << "-" << s << "-" << END << std::endl;
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
	else if ( s == "-k" )
		return ( s );
	sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE #lol +k <password>" ) );
	return ( "" );
}

static std::string	getPassword( Client &c, std::string s )
{
	std::string	password;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
		return ( "" );
	}
	if ( first_space != std::string::npos )
	{
		password = s.substr( first_space + 1 );
		if ( password.empty() )
		{
			sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE #lol +k <password>" ) );
			return ( "" );
		}
	}
	return ( password );
}
