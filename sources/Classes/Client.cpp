/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/04 16:23:43 by hgeffroy         ###   ########.fr       */
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

int Client::getCmd(std::string buffer)
{
	const std::string cmds[5] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN"};

	int end = static_cast<int>(buffer.find(' '));
	std::string cmd = buffer.substr(0, end);
	std::cout << "cmd is: " << cmd << std::endl;

	int i = 0;
	while (i < 5)
	{
		if (buffer == cmds[i])
			break;
		i++;
	}
	if (i == 5)
		return (-1);
	return (i);
}

void	Client::setPass(std::string& s, std::string& serverPass)
{
	if (_passwordOk)
		std::cout << "Password already entered" << std::endl;
	if (s.substr(5) == serverPass) // Whitespaces ?
	{
		_passwordOk = true;
		std::cout << "Correct password" << std::endl;
	}
	else
		std::cout << "Incorrect password" << std::endl;
}

void	Client::setNick(std::string s)
{
	if (_passwordOk)
	{
		_nickname = s.substr(5); // Whitespaces ?
		std::cout << "Nickname set to: " << _nickname << std::endl;
	}
	else
		std::cout << "Please enter the password first" << std::endl;
}

void	Client::setUser(std::string s)
{
	if (_passwordOk)
	{
		_username = s.substr(5);
		std::cout << "Username set to: " << _username << std::endl;
	}
	else
		std::cout << "Please enter the password first" << std::endl;
}


int	Client::setInfos(std::string serverPass) // Faire avec le getcmd et un switch
{
	std::string str = _bufRead;
	int cmd = getCmd(str);

	std::cout << "cmd is in int: " << cmd << std::endl;
	std::cout << "Setting infos" << std::endl;

	switch(cmd)
	{
		case PASS:
			setPass(str, serverPass);
		case NICK:
			setNick(str);
		case USER:
			setUser(str);
		default:
			; // Faire une gestion d'erreur
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
	else if (!_connected)
		setInfos(pass);
	else // Verifier la commande
	{
		int cmd = getCmd(_bufRead);
		for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
			if (it->getType() == FD_CLIENT && &(*it) != this) // Il faudra verifier quels destinataires on vise, pour l'instant on envoie a tous y compris les non connnectes et la commande !!
				send(it->getFd(), _bufRead, r, 0);
	}
	std::memset( _bufRead, 0, BUFFER_SIZE); // On vide le buffer !
}