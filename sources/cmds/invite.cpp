/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:42 by twang             #+#    #+#             */
/*   Updated: 2023/12/15 11:14:50 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static std::string	getNickName( std::string params );
static std::string	getChannelName( std::string params );

void	invite( Server& s, Client& c, std::string& params )
{
	std::string	nickname = getNickName( params );
	std::string	channel = getChannelName( params );
	std::cout << params << std::endl;
}

static std::string	getNickName( std::string params )
{
	std::string	nickname;
	std::size_t	first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		nickname = params.substr( first_space + 1, second_space - 1 );
		if ( nickname.empty( ) )
			return( "" );
		std::cout << YELLOW << nickname << END << std::endl;
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
	if ( second_space != std::string::npos )
	{
		channelname = params.substr( second_space + 1 );
		if ( channelname.empty( ) )
			return( "" );
		std::cout << RED << channelname << END << std::endl;
	}
	else
		return( "" );

	return ( channelname );
}

