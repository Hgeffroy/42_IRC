/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/10 09:44:06 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Client::Client(int socket) : _fd(socket), _connected(false), _passwordOk(false), _away(false), _nickname(""), _username("")
{
	std::memset( _bufRead, 0, BUFFER_SIZE);
	std::memset( _bufWrite, 0, BUFFER_SIZE);
	std::cout << "Salut je suis le constructeur de Client" << std::endl;
}

Client::~Client()
{
	std::cout << "Salut je suis le destructeur de Client" << std::endl;
}

/**  Setters and getters  *********************************************************************************************/

const char*	Client::getBufWrite() const
{
	return (_bufWrite);
}

int Client::getFd() const
{
	return (_fd);
}

std::string Client::getNick() const
{
	return (_nickname);
}

std::string Client::getUser() const
{
	return (_username);
}

/**  Private member functions  ****************************************************************************************/

int Client::getCmd(std::string& buffer)
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

void	Client::setPass(std::string& str, Server& s)
{
	if (_connected)
	{
		sendToClient(_fd, ERR_ALREADYREGISTERED(_nickname));
		return ;
	}

	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string pass;
	if (nextSpace == static_cast<int>(std::string::npos))
		pass = str.substr(5);
	else
		pass = str.substr(5, nextSpace - 5);

	std::cout << "Pass is: " << pass << " of size " << pass.size() << std::endl;

	if (pass.empty())
	{
		sendToClient(_fd, ERR_NEEDMOREPARAMS(_nickname, "PASS"));
		return ;
	}

	if (pass == s.getPass())
	{
		_passwordOk = true;
		std::cout << "Correct password" << std::endl;
	}
	else
		sendToClient(_fd, ERR_PASSWDMISMATCH(_nickname)); // May close connection
}

void	Client::setNick(std::string str, Server& s) // Verifier qu'il n'y a pas de doublon ?
{
	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string nick;
	if (nextSpace == static_cast<int>(std::string::npos))
		nick = str.substr(5);
	else
		nick = str.substr(5, nextSpace - 5);

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNick() == nick)
		{
			sendToClient(_fd, ERR_NICKNAMEINUSE(_nickname, nick));
			return ;
		}
	}

	if (nick.empty())
	{
		sendToClient(_fd, ERR_NONICKNAMEGIVEN(_nickname));
		return ;
	}

	_nickname = nick; // Whitespaces ?
	std::cout << "Nickname set to: " << _nickname << std::endl;
}

void	Client::setUser(std::string str, Server& s) // Doublon ?
{
	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string usr;
	
	if (nextSpace == static_cast<int>(std::string::npos))
		usr = str.substr(5);
	else
		usr = str.substr(5, nextSpace - 5);

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getUser() == usr)
		{
			sendToClient(_fd, ERR_ALREADYREGISTERED(_nickname));
			return ;
		}
	}

	if (usr.empty())
	{
		sendToClient(_fd, ERR_NEEDMOREPARAMS(_nickname, "USER"));
		return ;
	}

	_username = usr;
	std::cout << "Username set to: " << _username << std::endl;
}


int	Client::setInfos(Server& s, std::string& str)
{
	int cmd = getCmd(str);
	// if (cmd < 0)

	switch(cmd)
	{
		case PASS:
			setPass(str, s);
			break ;
		case NICK:
			setNick(str, s);
			break ;
		case USER:
			setUser(str, s);
			break ;
		default:
			; // Faire une gestion d'erreur
	}

	if (_passwordOk && !_username.empty() && !_nickname.empty()) // Faire ca dans la classe Server !!
	{
		std::map<std::string, Client*> clients = s.getClients();

		clients[_nickname] = this; // ou &(*this) ?
		// Delete de newClients.
		_connected = true;
		sendToClient(_fd, RPL_WELCOME(_nickname, _nickname, _username, getIP()));
		sendToClient(_fd, RPL_YOURHOST(_nickname, s.getName()));
		sendToClient(_fd, RPL_CREATED(_nickname, getTime(s)));
	}
	return (0);
}

void	Client::sendDM(Server& s, std::string& dest, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();
	std::cout << _nickname << " tries to send a msg to client " << dest << std::endl;

	std::string	fullMsg = ":" + _nickname + " PRIVMSG " + dest + " :" + msg;
	sendToClient(clients[dest]->_fd, fullMsg);
	if (clients[dest]->_away)
		sendToClient(_fd, RPL_AWAY(_nickname, clients[dest]->_nickname)); // Le RPL away ne permet pour l'instant pas de set le message away
				
	// sendToClient(_fd, ERR_NOSUCHNICK(_nickname, dest));
}

void	Client::sendChan(Server& s, std::string& dest, std::string& msg)
{
	std::vector<Channel*>	channels = s.getChannels();
	std::cout << _nickname << " tries to send a msg to channel " << dest << std::endl;
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == dest)
		{
			std::map<std::string, std::string>	members = (*it)->getMembers();
			for (std::map<std::string, std::string>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
			{
				std::string	fullMsg = ":" + _nickname + " PRIVMSG " + dest + " :" + msg; // Ce msg est pas bon
				sendToClient(s.getClientFd(it2->first), fullMsg);
			}
			return ;
		}
	}
	sendToClient(_fd, ERR_CANNOTSENDTOCHAN(_nickname, dest));
}

void	Client::sendBroadcast(Server& s, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();
	std::cout << _nickname << " tries to send a msg in broadcast" << std::endl;
	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) // Send to one client
	{
		std::string	fullMsg = ":" + _nickname + " PRIVMSG " + it->first+ " :" + msg;
		sendToClient(it->second->_fd, msg);
	}
}

void	Client::sendMsg(Server& s, std::string& str)
{
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections ici !

	std::string	dest = str.substr(sep1 + 1, sep2 - sep1 - 1);
	std::string msg = str.substr(sep2 + 1);

	if (msg.empty())
	{
		sendToClient(_fd, ERR_NOTEXTTOSEND(_nickname));
		return ;
	}

	if (std::isalpha(dest[0]))
		sendDM(s, dest, msg);
	else if (dest[0] == '#')
		sendChan(s, dest, msg);
	else if (dest[0] == '$')
		; // Send to all clients on server (broadcast)
}

void	Client::join(Server& s, std::string& str)
{
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections !!

	std::string	channelName = str.substr(sep1 + 1, sep2 - sep1 - 1);
	if (channelName[0] != '#')
		; // Send une erreur ici

	std::string fullMsg = ":" + _nickname + " JOIN " + channelName;
	sendToClient(_fd, fullMsg);

	std::vector<Channel*>			channels = s.getChannels();
	std::vector<Channel*>::iterator	it;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == channelName)
		{
			(*it)->addUser(*this);
			sendToClient(_fd, RPL_TOPIC(_nickname, channelName, (*it)->getTopic()));
			sendToClient(_fd, RPL_NAMREPLY(_nickname, "=", channelName, "@RandomUser")); // A changer !!
			sendToClient(_fd, RPL_ENDOFNAMES(_nickname, channelName));

			return ;
		}
	}
	Channel* newChannel = new Channel(channelName, _nickname);
	s.addChannel(newChannel);
	sendToClient(_fd, RPL_TOPIC(_nickname, channelName, newChannel->getTopic()));
	sendToClient(_fd, RPL_NAMREPLY(_nickname, "=", channelName, "@RandomUser")); // A changer !!
	sendToClient(_fd, RPL_ENDOFNAMES(_nickname, channelName));
}

std::vector<std::string>	Client::splitBuf()
{
	std::string 				buffer = _bufRead;
	std::vector<std::string>	res;

	int	sep1 = static_cast<int>(buffer.find('\n'));
	int	sep2 = static_cast<int>(buffer.find("\r\n"));
	int	prev = 0;
	int sep = std::min(sep1, sep2);
	res.push_back(buffer.substr(prev, sep));

	std::cout << "res[0] = " << res[0] << std::endl;

	int i = 1; // Securite a enlever quand ca marchera.

	while (sep != static_cast<int>(std::string::npos) && i < 10)
	{
		prev = sep1;
		sep1 = static_cast<int>(buffer.find('\n', sep1 + 1));
		sep2 = static_cast<int>(buffer.find("\r\n", sep1 + 1));
		sep = std::min(sep1, sep2);

		res.push_back(buffer.substr(prev + 1, sep - prev - 1));
		std::cout << "res[" << i << "] = " << res[i] << std::endl;
		i++;
	}

	return (res);
}

void	Client::execCmd(Server &s, std::string& str)
{
	std::cout << "Executing cmd" << std::endl;

	if (!_connected)
		setInfos(s, str);

	else // Verifier la commande
	{
		int cmd = getCmd(str);
		if (cmd < 0 && str != "")
		{
			std::cout << "Error sent" << std::endl; // Actually need to send one
		}

		switch (cmd)
		{
			case PRIVMSG:
				sendMsg(s, str);
				break;
			case JOIN:
				join(s, str);
				break;
			default:
				break;
		}
	}
}

/**  Public member functions  *****************************************************************************************/

void	Client::sendToClient(int fd, std::string str) const
{
	send(fd, str.c_str(), str.length(), MSG_DONTWAIT + MSG_NOSIGNAL); // Check les flags
}

void	Client::read(Server& s) // Le serveur lit ce que lui envoit le client
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0); // Met un \n a la fin !

	if (r <= 0)
		s.delClient(_fd);

//	std::cout << "Received from client " << _fd << ": " << std::endl;
//	std::cout << _bufRead << std::endl;

	std::vector<std::string>	cmds = splitBuf();

//	std::cout << "After split: " << std::endl;
//	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
//		std::cout << *it << std::endl;

	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
		execCmd(s, *it);

	std::memset( _bufRead, 0, BUFFER_SIZE); // On vide le buffer !
}