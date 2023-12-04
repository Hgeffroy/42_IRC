/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/04 10:11:11 by hgeffroy         ###   ########.fr       */
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
	if (str.substr(0, 5) == "PASS ") // Whitespaces a skip ou pas ?
	{
		if (_passwordOk)
			std::cout << "Password already entered" << std::endl;
		if (str.substr(5) == pass)
		{
			_passwordOk = true;
			std::cout << "Correct password" << std::endl;
		}
		else
			std::cout << "Incorrect password" << std::endl;
	}
	else if (str.substr(0, 5) == "NICK ") // Whitespaces ?
	{
		if (_passwordOk)
			_nickname = str.substr(5);
		else
			std::cout << "Please enter the password first" << std::endl;
	}
	else if (_passwordOk && str.substr(0, 5) == "USER ") // Whitespaces ?
	{
		if (_passwordOk)
			_username = str.substr(5);
		else
			std::cout << "PLease enter the password first" << std::endl;	
	}
	if (_passwordOk && !_username.empty() && !_nickname.empty())
		_connected = true;
	return (0);
}

/**  Public member functions  *****************************************************************************************/

void	Client::write() // Le serveur ecrit au client
{
	std::cout << "Server sending to client" << std::endl;
	std::memset( _bufWrite, 0, BUFFER_SIZE);
}

void	Client::read(std::vector<Client>& clients, std::string pass) // Le serveur lit ce que lui envoit le client
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0); // Met un \n a la fin !
	_bufRead[std::strlen(_bufRead) - 1] = 0; // Correction du \n, on verra si on garde.

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
			if (it->getType() == FD_CLIENT && &(*it) != this) // Il faudra verifier quels destinataires on vise, pour l'instant on envoie a tous y compris les non connnectes !
				send(it->getFd(), _bufRead, r, 0);
	}
}