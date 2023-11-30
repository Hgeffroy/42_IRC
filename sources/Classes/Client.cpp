/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 16:21:04 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Client::Client(int type, int socket) : _type(type), _fd(socket), _connected(false), _passwordOk(false), _nickname(""), _username("")
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

const char*	Client::getBufRead() const
{
	return (_bufRead);
}

const char*	Client::getBufWrite() const
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

/**  Private member functions  ****************************************************************************************/

int	Client::setInfos(std::string pass)
{
	std::string str = _bufRead;

	std::cout << "Setting infos" << std::endl;
	if (str.substr(0, 4) == "PASS")
	{
		std::cout << "Setting password: " << str.substr(5) << std::endl;
		std::cout << "Good password: " << pass << std::endl;
		std::cout << "This is supposed to be a space:" << str.c_str()[4] << "END" << std::endl;
		std::cout << (str.c_str()[4] == ' ') << " ; " << (str.substr(5).c_str() == pass.c_str()) << std::endl;
		if (str.c_str()[4] == ' ' && str.substr(5) == pass)
		{
			_passwordOk = true;
			std::cout << "Correct password" << std::endl;
		}
	}
	return (0);
//	else if (_passwordOk && ...)
}

/**  Public member functions  *****************************************************************************************/

void	Client::write() // Le serveur ecrit au client
{
	std::cout << "Server sending to client" << std::endl;
	std::memset( _bufWrite, 0, BUFFER_SIZE);
}

void	Client::read(std::vector<Client>& clients, std::string pass) // Le serveur lit ce que lui envoit le client
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0);

	std::cout << "Client sending to server" << std::endl;
	if (r <= 0)
	{
		close(_fd);
		std::cout << "Client on socket " << _fd << " gone" << std::endl;
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (&(*it) == this)
			{
				it = clients.erase(it);
				break;
			}
		}
	}
	else if (_connected == false)
		setInfos(pass);
	else
	{
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
			if (it->getType() == FD_CLIENT && &(*it) != this)
				send(it->getFd(), _bufRead, r, 0); //
	}
}