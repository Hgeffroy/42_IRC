/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:48:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 12:40:05 by hgeffroy         ###   ########.fr       */
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

Server::Server(std::string portstr, std::string password)
{
	struct sockaddr_in	sin;
	int					port;
	int 				s;

	port = setPort(portstr);
	_password = setPassword(password);
	s = socket(PF_INET, SOCK_STREAM, 0); // Check le 0 (Check si SOCK_STREAM n'a qu'un seul protocole), a recup !!
	if (s < 0)
		throw std::exception();
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	if (bind(s, reinterpret_cast< sockaddr* >(&sin), sizeof(sin)) < 0)
		throw std::exception();
	if (listen(s, 128) < 0)
		throw std::exception();
	_clients.push_back(Client(FD_SERV, s));
}

/**  Setters and Getters  *********************************************************************************************/

std::vector<Client>&	Server::getClients()
{
	return (_clients);
}

std::vector<Channel>&	Server::getChannels()
{
	return (_channels);
}

std::string	Server::getPass() const
{
	return (_password);
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

void	Server::accept(Client& client)
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len = sizeof(csin);

	cs = ::accept(client.getFd(), reinterpret_cast< struct sockaddr* >(&csin), &csin_len);
	std::cout << "New client on socket: " << cs << std::endl;
	_clients.push_back(Client(FD_CLIENT, cs));
}

int	Server::higherFd() const
{
	int									max = 0;
	std::vector<Client>::const_iterator	it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
		max = std::max(max, it->getFd());

	return (max);
}

/**  Public member functions  *****************************************************************************************/

void	Server::clientLeave(int fd)
{
	close(fd);
	std::cout << "Client on socket " << fd << " gone" << std::endl;
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			it = _clients.erase(it); // Verifier qu'on a bien efface sur le serveur !
			break;
		}
	}
}

void	Server::initFd()
{
	std::vector<Client>::iterator	it;

	FD_ZERO(&_fdRead);
	FD_ZERO(&_fdWrite);
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		FD_SET(it->getFd(), &_fdRead);
		if (std::strlen(it->getBufWrite()))
			FD_SET(it->getFd(), &_fdWrite);
	}
}

void	Server::checkFd()
{
	std::vector<Client>::iterator	it;
	int	i = select(static_cast<int>(higherFd()) + 1, &_fdRead, &_fdWrite, NULL, NULL); // Faire une fonction pour le premier argument.

	for (it = _clients.begin(); it != _clients.end() && i > 0; ++it)
	{
		if (FD_ISSET(it->getFd(), &_fdRead))
		{
			if (it->getType() == FD_SERV)
				accept(*it);
			else if (it->getType() == FD_CLIENT)
				it->read(*this); // Continue si le read a fait sortir le client !!
			i--;
		}
		if (FD_ISSET(it->getFd(), &_fdWrite))
		{
			i--;
//			it->write();
		}
	}
}
