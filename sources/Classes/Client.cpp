/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 08:36:50 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Client::Client() : _type(0)
{

}

Client::Client(int type, int socket) : _type(type), _fd(socket)
{

}

Client::~Client()
{

}

/**  Setters and getters  *********************************************************************************************/

int Client::getType() const
{
	return (_type);
}

std::string	Client::getBufRead() const
{
	return (_bufRead);
}

std::string	Client::getBufWrite() const
{
	return (_bufWrite);
}

int Client::getFd() const
{
	return (_fd);
}


void	Client::setType(int newType)
{
	_type = newType;
}

/**  Public member functions  *****************************************************************************************/

void	Client::write()
{
	std::cout << "Client writing" << std::endl;
}

void	Client::read()
{
	std::cout << "Client reading" << std::endl;
}