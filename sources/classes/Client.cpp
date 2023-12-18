/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 14:07:47 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Client::Client(int socket) : _fd(socket), _connected(false), _passwordOk(false), _away(false), _nickname(""),
							 _username("")
{
	std::memset(_bufRead, 0, BUFFER_SIZE);
	std::memset(_bufWrite, 0, BUFFER_SIZE);
	std::cout << "Salut je suis le constructeur de Client" << std::endl;
}

Client::~Client()
{
	close(_fd);
	std::cout << "Salut je suis le destructeur de Client" << std::endl;
}

/**  Setters and getters  *********************************************************************************************/

const char *Client::getBufWrite() const
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

bool Client::getConnected() const
{
	return (_connected);
}

bool Client::getAway() const
{
	return (_away);
}

void Client::setPassOk()
{
	_passwordOk = true;
}

void Client::setUser(std::string &str)
{
	_username = str;
}

void Client::setNick(std::string &str)
{
	_nickname = str;
}


/**  Private member functions  ****************************************************************************************/

int Client::getCmd(std::string &buffer)
{
	const int nbcmd = 11;
	const std::string cmds[nbcmd] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN", "MODE", "WHO", "PART", "QUIT",
									 "INVITE", "TOPIC"};

	int end = static_cast<int>(buffer.find(' '));
	std::string cmd = buffer.substr(0, end);

	int i = 0;
	while (i < nbcmd)
	{
		if (cmd == cmds[i])
			break;
		i++;
	}
	if (i == nbcmd)
		return (-1);
	return (i);
}

int Client::setInfos(Server &s, std::string &str)
{
	int cmd = getCmd(str);
	// if (cmd < 0)

	switch (cmd)
	{
		case PASS:
			pass(s, *this, str);
			break;
		case NICK:
			nick(s, *this, str);
			break;
		case USER:
			user(s, *this, str);
			break;
		default:
			int end = static_cast<int>(str.find(' '));
			std::string cmdStr = str.substr(0, end);
			sendToClient(this->getFd(), ERR_UNKNOWNCOMMAND(this->getNick(), cmdStr));
			break;
	}

	if (_passwordOk && !_username.empty() && !_nickname.empty()) // Faire ca dans la classe Server !!
	{
		s.addClient(this);

		_connected = true;
		sendToClient(_fd, RPL_WELCOME(_nickname, _nickname, _username, getIP()));
		sendToClient(_fd, RPL_YOURHOST(_nickname, s.getName()));
		sendToClient(_fd, RPL_CREATED(_nickname, getTime(s)));
		sendToClient(_fd, RPL_MYINFO(_nickname, s.getName()));
		sendToClient(_fd, RPL_ISUPPORT(_nickname, "10", "50")); // A changer avec le define
	}
	return (0);
}

std::vector<std::string> Client::splitBuf()
{
	std::string buffer = _bufRead;
	std::vector<std::string> res;

	size_t sep1 = buffer.find('\n');
	size_t sep2 = buffer.find("\r\n");
	size_t prev = 0;
	size_t sep = std::min(sep1, sep2);
	std::string tempStr = buffer.substr(prev, sep - prev);

	if (tempStr.length() < 1)
		return (res);
	res.push_back(tempStr);

	std::cout << "res[0] =" << res[0] << "=" << std::endl;

	int i = 1;
	while (sep != std::string::npos && i < 10)
	{
		while (buffer[sep] == '\r' || buffer[sep] == '\n')
			sep++;

		prev = sep;

		sep1 = buffer.find('\n', sep);
		sep2 = buffer.find("\r\n", sep);
		sep = std::min(sep1, sep2);

		tempStr = buffer.substr(prev, sep - prev);
		if (tempStr.length() < 1)
			break;

		res.push_back(tempStr);
		std::cout << "res[" << i << "] =" << res[i] << "=" << std::endl;
		i++;
	}

	return (res);
}

void Client::execCmd(Server &s, std::string &str)
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
			case NICK:
				nick(s, *this, str);
				break;
			case PRIVMSG:
				sendMsg(s, *this, str);
				break;
			case JOIN:
				join(s, *this, str);
				break;
			case MODE:
				mode(s, *this, str);
				break;
			case WHO:
				who(s, *this, str);
				break;
			case PART:
				part(s, *this, str);
			case QUIT:
				quit(s, *this, str);
				break;
			case INVITE:
				invite(s, *this, str);
				break;
			case TOPIC:
				topic(s, *this, str);
				break;
			default:
				std::size_t end = str.find(' ');
				std::string cmdStr = str.substr(0, end);
				sendToClient(this->getFd(), ERR_UNKNOWNCOMMAND(this->getNick(), cmdStr));
				break;
		}
	}
}

/**  Public member functions  *****************************************************************************************/

int Client::read(Server &s) // Le serveur lit ce que lui envoit le client
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0); // Met un \n a la fin !

	if (r <= 0)
	{
		s.removeClient(*this);
		return (1);
	}

	std::cout << RED << "From " << _fd << ": " << _bufRead << END << std::endl;

	std::vector<std::string> cmds = splitBuf();

//	std::cout << "After split: " << std::endl;
//	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
//		std::cout << *it << std::endl;

	for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
		execCmd(s, *it);

	std::memset(_bufRead, 0, BUFFER_SIZE); // On vide le buffer !

	return (0);
}