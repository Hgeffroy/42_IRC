/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:05:57 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/03 14:23:12 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

// void	part(Server& s, Client& c, std::string& str) // Need more params
// {
// 	std::size_t	space1 = str.find(' ');
// 	std::size_t	space2 = str.find(' ', space1 + 1);

// 	if (space1 == std::string::npos || space2 == std::string::npos)
// 	{
// 		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PART"));
// 		return ;
// 	}

// 	std::string	chan = str.substr(space1 + 1, space2 - space1 - 1);

// 	std::map<std::string, Channel*>				channels = s.getChannels();
// 	std::map<std::string, Channel*>::iterator	itChan = channels.find(chan);

// 	if (itChan == channels.end())
// 	{
// 		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), chan));
// 		return ;
// 	}

// 	std::map<std::string, std::string>				members = itChan->second->getMembers();
// 	std::map<std::string, std::string>::iterator	itMembers = members.find(c.getNick());

// 	if (itMembers == members.end())
// 	{
// 		sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), chan));
// 		return ;
// 	}

// 	itChan->second->removeUserFromChan(c.getNick());
// 	//nous avons un part qui detruit le client .
// }

static std::vector< std::string >	getNames( std::string s );
static std::string					getReason( std::string s );

void	part( Server& s, Client& c, std::string& str )
{
	std::map< std::string, Channel* >	channels = s.getChannels( );
	std::vector< std::string >			names = getNames( str );
	std::string							reason = getReason( str );
	
	std::cout << YELLOW << "reason -" << reason << "-" << END << std::endl;
	

}
	// PART #lol,#pop je suis fatiguee
		//	channels  raison

static std::vector< std::string >	getNames( std::string s )
{
	std::vector< std::string >	channels;
	std::string					names;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		names = s.substr( first_space + 1, second_space - first_space - 1 );
		std::size_t	first_comma = names.find( ',' );
		std::size_t	second_comma = names.find( ',', first_comma + 1 );
		if ( first_comma != std::string::npos )
		{
			channels.push_back( names.substr( 0, first_comma ) );
			if ( second_comma != std::string::npos )
				channels.push_back( names.substr( first_comma + 1, second_comma - first_comma - 1 ) );
			channels.push_back( names.substr( second_comma + 1 ) );
			for (std::vector<std::string>::iterator it = channels.begin() ; it != channels.end(); ++it )
				std::cout << PURPLE << *it << std::endl;

		}

			// channels.push_back( names );
		return ( channels );
	}
	names = s.substr( first_space + 1 );
	return ( channels );
}

static std::string	getReason( std::string s )
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
