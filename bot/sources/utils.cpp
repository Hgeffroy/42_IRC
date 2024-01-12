/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 14:40:33 by twang             #+#    #+#             */
/*   Updated: 2024/01/12 16:58:09 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::vector< std::string >	Bot::splitBuffer( void )
{
	std::string					buffer = _bufRead;
	std::vector<std::string>	res;

	size_t sep = buffer.find("\r\n");
	size_t prev = 0;
	std::string tempStr = buffer.substr(prev, sep - prev);

	if (tempStr.length() < 1)
		return (res);
	if (tempStr[tempStr.length() - 1] == ' ')
		tempStr = tempStr.substr(0, tempStr.length() - 1);
	res.push_back(tempStr);

	while (sep != std::string::npos)
	{

		while (buffer[sep] == '\r' || buffer[sep] == '\n')
			sep++;

		prev = sep;
		sep = buffer.find("\r\n", prev);
		tempStr = buffer.substr(prev, sep - prev);

		if (tempStr.length() < 1)
			break;
		if (tempStr[tempStr.length() - 1] == ' ')
			tempStr = tempStr.substr(0, tempStr.length() - 1);
		res.push_back(tempStr);
	}

	return (res);
}

std::string	splitCommand( std::string &buffer )
{
	std::size_t	first_space = buffer.find( ' ' );
	std::size_t	second_space = buffer.find( ' ', first_space + 1 );
	std::string	command = buffer.substr( first_space + 1, second_space - first_space - 1 );
	return ( command );
}

std::string	splitMessage( std::string &buffer )
{
	std::size_t	first_space = buffer.find( ' ' );
	std::size_t	second_space = buffer.find( ' ', first_space + 1 );
	std::size_t	colon = buffer.find( ':', second_space + 1 );
	std::string	message = buffer.substr( colon + 1 );

	return ( message );
}

std::vector< std::string >	splitArguments( std::string &s )
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
