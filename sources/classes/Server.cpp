/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:48:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 14:03:19 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  *********************************************/

Server::Server() {}

Server::~Server()
{
	std::map<std::string, Client*>::iterator	itClients;
	std::map<std::string, Channel*>::iterator	itChannel;
	std::vector<Client*>::iterator				itNewClients;

	for (itChannel = _channels.begin(); itChannel != _channels.end(); ++itChannel)
		delete itChannel->second;

	for (itClients = _clients.begin(); itClients != _clients.end(); ++itClients)
		delete itClients->second;

	for (itNewClients = _newClients.begin(); itNewClients != _newClients.end(); ++itNewClients)
		delete *itNewClients;

	close(_listener);
}

Server::Server(std::string portstr, std::string password) : _creationTime(time(0)), _name("MyIrcServ"), _motd("I like Mr Freeze")
{
	struct sockaddr_in	sin;
	int					port;
	int					s;
	int					optval = 1;


	close(STDIN_FILENO);
	port = setPort(portstr);
	_password = setPassword(password);
	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0)
		throw std::runtime_error("socket failed");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	if (bind(s, reinterpret_cast< sockaddr* >(&sin), sizeof(sin)) < 0)
		throw std::runtime_error("bind failed");
	if (listen(s, 128) < 0)
		throw std::runtime_error("listen failed");
	_listener = s;
}

/**  Setters and Getters  ******************************************************/

std::map<std::string, Client*>	Server::getClients() const
{
	return (_clients);
}

std::map<std::string, Channel*>	Server::getChannels() const
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

std::string Server::getMotd() const
{
	return (_motd);
}

time_t* 	Server::getCreationTime()
{
	return (&_creationTime);
}

/**  Private member functions  *************************************************/

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

void	Server::acceptClient()
{
	int					cs;
	struct sockaddr_in	csin;
	socklen_t			csin_len = sizeof(csin);

	cs = ::accept(_listener, reinterpret_cast< struct sockaddr* >(&csin), &csin_len);
	if ( cs < 0 )
		throw std::invalid_argument( "<accept> cannot connect to the server" );
	std::cout << "New client on socket: " << cs << std::endl;
	Client* newClient = new Client(cs);
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

void	Server::removeClientFromChannels(Client& c)
{
	std::map<std::string, Channel*>::iterator	it;

	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::map<std::string, std::string>				members = it->second->getMembers();
		std::map<std::string, std::string>::iterator	find = members.find(c.getNick());
		if (find != members.end())
		{
			if (it->second->removeUserFromChan(*this, find->first)) {
				removeClientFromChannels(c);
				return ;
			}
			it->second->sendToChannel(*this, PART_MSG(c.getNick(), c.getUser(), it->second->getName(), "Disconnected"));
		}
	}
}

/**  Public member functions  **************************************************/

int Server::getClientFd(std::string nickname)
{
	if (_clients.find(nickname) == _clients.end())
		return (-1);
	return (_clients[nickname]->getFd());
}

void	Server::switchNick(std::string& prevNick, std::string& newNick)
{
	_clients[newNick] = _clients[prevNick];
	_clients.erase(prevNick);

	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		it->second->switchNick(prevNick, newNick);
	}
}


void	Server::addClient(Client *client)
{
	_clients[client->getNick()] = client;

	std::vector<Client*>::iterator	it;
	for (it = _newClients.begin(); it != _newClients.end(); ++it)
		if (*it == client)
		{
			_newClients.erase(it);
			return ;
		}
}

void	Server::removeClient(Client& c)
{
	close(c.getFd());
	std::cout << "Client on socket " << c.getFd() << " gone." << std::endl;
	removeClientFromChannels(c);

	for (std::vector<Client*>::iterator it = _newClients.begin(); it != _newClients.end(); ++it)
	{
		if (*it== &c)
		{
			delete *it;
			_newClients.erase(it);
			break;
		}
	}

	for (std::map<std::string, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second == &c)
		{
			delete it->second;
			_clients.erase(it->first);
			break;
		}
	}
}

void	Server::addChannel(Channel* newChannel)
{
	_channels[newChannel->getName()] = newChannel;
}

void	Server::removeChannel(Channel* chan)
{
	std::map< std::string, std::string >			_members = chan->getMembers();
	std::map< std::string, std::string >::iterator	it;
	for ( it = _members.begin(); it != _members.end(); ++it ) {
		chan->sendToChannel( *this, PART_MSG( it->first, it->first, chan->getName( ), "Channel deleted" ) );
	}

	if (_channels.find(chan->getName()) != _channels.end()) {
		_channels.erase(chan->getName());
		delete chan;
	}
}

void	Server::initFd()
{
	std::vector<Client*>::iterator				it;
	std::map<std::string, Client*>::iterator	it2;

	FD_ZERO(&_fdRead);

	FD_SET(_listener, &_fdRead);

	for (it = _newClients.begin(); it != _newClients.end(); ++it)
		FD_SET((*it)->getFd(), &_fdRead);

	for (it2 = _clients.begin(); it2 != _clients.end(); ++it2)
		FD_SET(it2->second->getFd(), &_fdRead);
}

void	Server::checkFd()
{
	int	i = select(static_cast<int>(higherFd()) + 1, &_fdRead, NULL, NULL, NULL);
	if (i < 0) {
		throw std::runtime_error("Select failed");
	}

	if (FD_ISSET(_listener, &_fdRead)) {
		acceptClient();
		i--;
	}


	std::vector<Client*>::iterator				it;
	std::map<std::string, Client*>::iterator	it2;
	
	for (it2 = _clients.begin(); it2 != _clients.end() && i > 0; ++it2) {
		if (FD_ISSET(it2->second->getFd(), &_fdRead)) {
			if (it2->second->read(*this) == 1)
				break;
			i--;
		}
	}

	for (it = _newClients.begin(); it != _newClients.end() && i > 0; ++it) {
		if (FD_ISSET((*it)->getFd(), &_fdRead)) {
			if ((*it)->read(*this) == 1)
				break;
			i--;
		}
	}
}
