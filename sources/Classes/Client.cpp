/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 14:21:17 by hgeffroy         ###   ########.fr       */
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

	int i = 0;
	while (i < 5)
	{
		if (cmd == cmds[i])
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

void	Client::setNick(std::string s) // Verifier qu'il n'y a pas de doublon ?
{
	if (_passwordOk)
	{
		_nickname = s.substr(5); // Whitespaces ?
		std::cout << "Nickname set to: " << _nickname << std::endl;
	}
	else
		std::cout << "Please enter the password first" << std::endl;
}

void	Client::setUser(std::string s) // Doublon ?
{
	if (_passwordOk)
	{
		_username = s.substr(5);
		std::cout << "Username set to: " << _username << std::endl;
	}
	else
		std::cout << "Please enter the password first" << std::endl;
}


int	Client::setInfos(std::string serverPass)
{
	std::string str = _bufRead;
	int cmd = getCmd(str);
	// if (cmd < 0)

	switch(cmd)
	{
		case PASS:
			setPass(str, serverPass);
			break ;
		case NICK:
			setNick(str);
			break ;
		case USER:
			setUser(str);
			break ;
		default:
			; // Faire une gestion d'erreur
	}

	if (_passwordOk && !_username.empty() && !_nickname.empty())
		_connected = true;
	return (0);
}

void	Client::sendMsg(std::vector<Client>& c)
{
	std::string str = _bufRead;
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections ici !

	std::string	dest = str.substr(sep1 + 1, sep2 - sep1 - 1);
	if (std::isalpha(dest[0]))
	{
		std::cout << _nickname << " tries to send a msg to " << dest << std::endl;
		for (std::vector<Client>::iterator it = c.begin(); it != c.end(); ++it)
		{
			if (it->_nickname == dest && it->_connected) // Envoyer une erreur si le destinataire existe pas
			{
				send(it->getFd(), _bufRead + sep2 + 1, str.length() - sep2 - 1, 0); // C'est du jamais vu
				send(it->getFd(), "\n", 1, 0);
			}
		}
	}
//	else if ()
}

void	Client::join(std::vector<Channel>& channels)
{

}

/**  Public member functions  *****************************************************************************************/

void	Client::write() // Le serveur ecrit au client
{
	std::cout << "Server sending to client" << std::endl;
	std::memset( _bufWrite, 0, BUFFER_SIZE);
}

void	Client::read(Server& s) // Le serveur lit ce que lui envoit le client
{
	std::vector<Client>&	c = s.getClients();
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0); // Met un \n a la fin !
	_bufRead[std::strlen(_bufRead) - 1] = 0; // Correction du \n, on verra si on garde.

	if (r <= 0)
		s.clientLeave(_fd);
	else if (!_connected)
		setInfos(s.getPass());

	else // Verifier la commande
	{
		int cmd = getCmd(_bufRead);
		if (cmd < 0)
		{
			std::cout << "Error sent" << std::endl;
			send(this->getFd(), "CMD DOES NOT EXIST\n", 20, 0);
		}


		switch(cmd)
		{
			case PRIVMSG:
				sendMsg(c);
				break ;
			case JOIN:
				join(s.getChannels());
				break;
			default:
				break ;
		}


	}
	std::memset( _bufRead, 0, BUFFER_SIZE); // On vide le buffer !
}