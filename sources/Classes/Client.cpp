/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 11:25:33 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Client::Client() : _type(0)
{
	std::memset( _bufRead, 0, BUFFER_SIZE);
	std::memset( _bufWrite, 0, BUFFER_SIZE);
}

Client::Client(int type, int socket) : _type(type), _fd(socket)
{
	std::memset( _bufRead, 0, BUFFER_SIZE);
	std::memset( _bufWrite, 0, BUFFER_SIZE);
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

void	Client::read(std::vector<Client> clients)
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0);

	std::cout << "Client reading" << std::endl;
	if (r <= 0)
	{
		close(_fd);
		std::cout << "Client gone" << std::endl;
	}
	else
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			if (it->getType() == FD_CLIENT && &(*it) != this)
				send(it->getFd(), _bufRead, r, 0);
		}
	}
}