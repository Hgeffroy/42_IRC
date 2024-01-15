/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:05:57 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 13:41:47 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::vector< std::string >	getNames( std::string& s );
static std::string					getReason( std::string& s );

/*----------------------------------------------------------------------------*/

void	part( Server& s, Client& c, std::string& str )
{
	std::map< std::string, Channel* >	channels = s.getChannels( );
	std::vector< std::string >			names = getNames( str );
	std::string							reason = getReason( str );

	if ( names.empty( ) )
		sendToClient( c.getFd( ), ERR_NEEDMOREPARAMS( c.getNick( ), "PART" ) );

	for (std::vector< std::string >::iterator it = names.begin( ) ; it != names.end( ); ++it )
	{
		std::map< std::string, Channel* >::iterator	itc = channels.find(*it);
		if ( itc != channels.end( ) )
		{
			std::map<std::string, std::string>				members = itc->second->getMembers( );
			std::map<std::string, std::string>::iterator	itm = members.find( c.getNick( ) );
			if ( itm == members.end( ) )
				sendToClient( c.getFd(), ERR_NOTONCHANNEL( c.getNick( ), *it) );
			else
			{
				itc->second->sendToChannel( s, PART_MSG( c.getNick( ), c.getUser( ), itc->second->getName( ), reason ) );
				itc->second->removeUserFromChan( s, c.getNick( ) );
			}
		}
		else
			sendToClient( c.getFd( ), ERR_NOSUCHCHANNEL( c.getNick( ), *it ) );
	}
}

static std::vector< std::string >	getNames( std::string& s )
{
	std::vector< std::string >	channels;
	std::string					names;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		std::string			str = s.substr( first_space + 1, second_space - first_space - 1 );
		std::istringstream	iss(str);
		while (std::getline(iss, names, ','))
			channels.push_back( names.substr(0) );
		return ( channels );
	}
	else if ( first_space != std::string::npos )
	{
		std::string			str = s.substr( first_space + 1 );
		std::istringstream	iss(str);
		while (std::getline(iss, names, ','))
			channels.push_back( names.substr(0) );
		return ( channels );
	}
	return ( channels );
}

static std::string	getReason( std::string& s )
{
	std::string	reason;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		reason = s.substr( second_space + 1 );
		return ( reason );
	}
	return ( "" );
}
