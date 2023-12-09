/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:48:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/09 16:05:50 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Server::Server()
{

}

Server::~Server()
{

}

Server::Server(std::string portstr, std::string password) :  _creationTime(time(0)), _name("MyIrcServ")
{
	struct sockaddr_in	sin;
	int					port;
	int 				s;

	port = setPort(portstr);
	_password = setPassword(password);
	s = socket(PF_INET, SOCK_STREAM, 0); // Check le 0 (Check si SOCK_STREAM n'a qu'un seul protocole), a recup !!
	if (s < 0)
		throw std::runtime_error("socket failed");
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	if (bind(s, reinterpret_cast< sockaddr* >(&sin), sizeof(sin)) < 0)
		throw std::runtime_error("bind failed");
	if (listen(s, 128) < 0)
		throw std::runtime_error("listen failed");
	_listener = s;
	// Client* newClient = new Client(FD_SERV, s);
	// _clients["listener"] = newClient;
}

/**  Setters and Getters  *********************************************************************************************/

std::map<std::string, Client*>	Server::getClients() const
{
	return (_clients);
}

std::vector<Client*>	Server::getNewClients() const
{
	return (_newClients);
}

std::vector<Channel*>	Server::getChannels() const
{
	return (_channels);
}

std::string	Server::getPass() const
{
	return (_password);
}

std::string	Server::getName() const
{
	return (_name);
}

time_t* 	Server::getCreationTime()
{
	return (&_creationTime);
}

/**  Private member functions  ****************************************************************************************/

int	Server::setPort(std::string& portstr)
{
	char* end;
	int	port = static_cast<int>(std::strtol(portstr.c_str(), &end, 10));
	if (errno == ERANGE || port == 0 || *end)
		throw std::invalid_argument("<port> is not an integer");
	if (port < 1024 || port > 65535)
		throw std::invalid_argument("<port> is not a valid port");
	return (port);
}

std::string	Server::setPassword(std::string& pass)
{
	if (pass.size() < 5)
		throw std::invalid_argument("<password> is too short");


	std::string::iterator	it;
	for (it = pass.begin(); it != pass.end(); ++it)
		if (!std::isprint(*it) || *it == ' ' || *it == '	')
			throw std::invalid_argument("<password> has an invalid character");

	return (pass);
}

void	Server::accept()
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len = sizeof(csin);

	cs = ::accept(_listener, reinterpret_cast< struct sockaddr* >(&csin), &csin_len);
	std::cout << "New client on socket: " << cs << std::endl;
	Client* newClient = new Client(FD_CLIENT, cs);
	_newClients.push_back(newClient);
}

int	Server::higherFd() const
{
	int												max = _listener;
	std::vector<Client*>::const_iterator			it;
	std::map<std::string, Client*>::const_iterator	it2;

	for (it = _newClients.begin(); it != _newClients.end(); ++it)
		max = std::max(max, (*it)->getFd());
	
	for (it2 = _clients.begin(); it2 != _clients.end(); ++it2)
		max = std::max(max, it2->second->getFd());

	return (max);
}

/**  Public member functions  *****************************************************************************************/

int Server::getClientFd(std::string nickname)
{
	return (_clients[nickname]->getFd());
}

void	Server::delClient(int fd) // Attention a bien del dans les chan aussi ? Normalement ok si je passe bien les refs ? Ou pas ?
{
	close(fd);
	std::cout << "Client on socket " << fd << " gone" << std::endl;
	
	for (std::vector<Client*>::iterator it = _newClients.begin(); it != _newClients.end(); ++it)
	{
		if ((*it)->getFd() == fd)
		{
			it = _newClients.erase(it); // Verifier qu'on a bien delete, pas de leaks.
			break;
		}
	}

	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getFd() == fd)
		{
			delete it->second;
			break;
		}
	}
	
}

void	Server::addChannel(Channel* newChannel)
{
	_channels.push_back(newChannel);
}

void	Server::initFd()
{
	std::vector<Client*>::iterator				it;
	std::map<std::string, Client*>::iterator	it2;

	FD_ZERO(&_fdRead);
	FD_ZERO(&_fdWrite);
	
	FD_SET(_listener, &_fdRead);
	
	for (it = _newClients.begin(); it != _newClients.end(); ++it)
	{
		FD_SET((*it)->getFd(), &_fdRead);
		if (std::strlen((*it)->getBufWrite()))
			FD_SET((*it)->getFd(), &_fdWrite);
	}

	for (it2 = _clients.begin(); it2 != _clients.end(); ++it2)
	{
		FD_SET(it2->second->getFd(), &_fdRead);
		if (std::strlen(it2->second->getBufWrite()))
			FD_SET(it2->second->getFd(), &_fdWrite);
	}
}

void	Server::checkFd()
{
	std::vector<Client*>::iterator				it;
	std::map<std::string, Client*>::iterator	it2;
	int	i = select(static_cast<int>(higherFd()) + 1, &_fdRead, &_fdWrite, NULL, NULL); // Faire une fonction pour le premier argument.

	if (FD_ISSET(_listener, &_fdRead))
	{
		accept();
		i--;
	}

	for (it = _newClients.begin(); it != _newClients.end() && i > 0; ++it)
	{
		if (FD_ISSET((*it)->getFd(), &_fdRead))
		{
			if ((*it)->getType() == FD_CLIENT)
				(*it)->read(*this); // Continue si le read a fait sortir le client !!
			i--;
		}
		if (FD_ISSET((*it)->getFd(), &_fdWrite))
		{
			i--;
//			it->write();
		}
	}

	for (it2 = _clients.begin(); it2 != _clients.end() && i > 0; ++it2)
	{
		if (FD_ISSET(it2->second->getFd(), &_fdRead))
		{
			if (it2->second->getType() == FD_CLIENT)
				it2->second->read(*this); // Continue si le read a fait sortir le client !!
			i--;
		}
		if (FD_ISSET(it2->second->getFd(), &_fdWrite))
		{
			i--;
//			it2->write();
		}
	}
}
