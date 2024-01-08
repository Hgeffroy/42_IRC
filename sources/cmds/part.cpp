/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:05:57 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/08 11:16:39 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::vector< std::string >	getNames( std::string& s );
static std::string					getReason( std::string& s );
//static void							sendToChannel( Server& s, Channel* chan, std::string str );


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
			if ( itm != members.end( ) )
			{
				std::cout << YELLOW << c.getNick( ) << " left channel " << *it;
				if ( !reason.empty( ) )
					std::cout << reason;
				std::cout << END << std::endl;
				itc->second->sendToChannel( s, PART_MSG( c.getNick( ), c.getUser( ), itc->second->getName( ), reason ) );
				itc->second->removeUserFromChan( s, c.getNick( ) );
			}
			else
				sendToClient(c.getFd(), ERR_NOTONCHANNEL( c.getNick( ), *it) );
		}
		else
			sendToClient( c.getFd( ), ERR_NOSUCHCHANNEL( c.getNick( ), *it ) );
	}

}

//static void	sendToChannel( Server& s, Channel* chan, std::string str )
//{
//	std::map< std::string, std::string > 			members = chan->getMembers( );
//	std::map< std::string, std::string >::iterator	it = members.begin( );
//	std::map< std::string, Client* > 				clients = s.getClients( );
//
//	for ( it = members.begin( ); it != members.end( ); ++it ) {
//		Client* client = clients.find( it->first )->second;
//		sendToClient( client->getFd( ), str );
//	}
//}

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
