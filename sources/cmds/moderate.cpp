/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moderate.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 08:56:27 by twang             #+#    #+#             */
/*   Updated: 2024/01/15 09:49:07 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::vector< std::string >	getNames( std::string& s );
static bool							isFounder( Client &c, Channel *channel );

/*----------------------------------------------------------------------------*/

void	moderator( Server& s, Client& c, std::string& str )
{
	std::string							bot = "bot";
	std::string							selected;
	std::map<std::string, Client*>		clients = s.getClients();
	std::vector< std::string >			names = getNames( str );
	std::map< std::string, Channel* >	channels = s.getChannels( );


	if ( names.empty( ) )
		sendToClient( c.getFd( ), ERR_NEEDMOREPARAMS( c.getNick( ), "MODERATE" ) );

	std::string	command = ":" + c.getNick() + " MODERATE" + " : ";
	for ( std::vector< std::string >::iterator		it = names.begin() ; it != names.end(); it++ )
	{
		std::map< std::string, Channel* >::iterator	itc = channels.find(*it);
		if ( itc != channels.end( ) )
		{
			std::map<std::string, std::string>				members = itc->second->getMembers( );
			std::map<std::string, std::string>::iterator	itm = members.find( c.getNick( ) );
			if ( itm != members.end( ) && isFounder( c, channels[*it] ) != false )
			{
				selected += *it;
				command += *it;
				if ( it != names.end() - 1 )
				{
					selected += ",";
					command += ",";
				}
			}
			else if ( itm == members.end( ) )
				sendToClient( c.getFd( ), ERR_NOTONCHANNEL( c.getNick( ), *it) );
		}
		else
			sendToClient( c.getFd( ), ERR_NOSUCHCHANNEL( c.getNick( ), *it ) );
	}
	if ( clients[bot] )
	{
		std::cout << YELLOW << c.getNick() << " is asking for moderation on channel : " << selected << END << std::endl;
		command += ENDLINE;
		sendToClient( clients[bot]->getFd(), command );
	}
	else
		sendToClient( c.getFd( ), ERR_UNKNOWNERROR( c.getNick( ), "MODERATE", "The bot is not available.") );
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

static bool	isFounder( Client &c, Channel *channel )
{
	std::map< std::string, std::string >members = channel->getMembers();
	if ( members[ c.getNick( ) ] != "~" )
	{
		sendToClient(c.getFd(), ERR_NOPRIVS( c.getNick() ) );
		return ( false );
	}
	return ( true );
}
