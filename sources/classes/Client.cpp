/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:51:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 14:02:25 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  *********************************************/

Client::Client(int socket) : _fd(socket), _connected(false), _passwordOk(false), \
							_away(false), _nickname(""), _username("")
{
	std::memset(_bufRead, 0, BUFFER_SIZE);
	std::memset(_bufWrite, 0, BUFFER_SIZE);
}

Client::~Client()
{
	close(_fd);
}

/**  Setters and getters  ******************************************************/

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

void Client::setPassNotOk()
{
	_passwordOk = false;
}

void Client::setUser(std::string &str)
{
	_username = str;
}

void Client::setNick(Server& s, std::string& str)
{
	std::string prev = _nickname;

	if (prev != str && _connected) {
		s.switchNick(prev, str);
	}
	_nickname = str;
}

void Client::setAway(bool away)
{
	_away = away;
}


/**  Private member functions  *************************************************/

int Client::getCmd(std::string &buffer)
{
	const int nbcmd = 19;
	const std::string cmds[nbcmd] = {"PASS", "NICK", "USER", "PRIVMSG", "JOIN", "MODE", "WHO", "PART", "QUIT",
									 "INVITE", "TOPIC", "MOTD", "PING", "LIST", "KICK", "AWAY", "BOT", "MODERATE", "moderate"};

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

	if (cmd < 0) {
		std::size_t end = str.find(' ');
		std::string cmdStr = str.substr(0, end);
		sendToClient(this->getFd(), ERR_UNKNOWNCOMMAND(this->getNick(), cmdStr));
	}

	switch (cmd)
	{
		case PASS:
			pass(s, *this, str);
			break;
		case NICK:
			nick(s, *this, str, false);
			break;
		case USER:
			user(s, *this, str, false);
			break;
		case BOT:
			bot(s, *this, str);
			break;
		default:
			sendToClient(this->getFd(), ERR_NOTREGISTERED(this->getNick()));
			break;
	}

	if (_passwordOk && !_username.empty() && !_nickname.empty())
	{
		s.addClient(this);

		_connected = true;
		motd(s, *this);
		sendToClient(_fd, RPL_WELCOME(_nickname, _nickname, _username));
		sendToClient(_fd, RPL_YOURHOST(_nickname, s.getName()));
		sendToClient(_fd, RPL_CREATED(_nickname, getTime(s)));
		sendToClient(_fd, RPL_MYINFO(_nickname, s.getName()));
		sendToClient(_fd, RPL_ISUPPORT(_nickname, NICKLEN, CHANNELEN));
	}
	return (0);
}

std::vector<std::string> Client::splitBuf()
{
	std::vector<std::string> res;

	size_t sep = _buffer.find("\r\n");
	size_t prev = 0;
	std::string tempStr = _buffer.substr(prev, sep - prev);

	if (tempStr.length() < 1)
		return (res);
	if (tempStr[tempStr.length() - 1] == ' ')
		tempStr = tempStr.substr(0, tempStr.length() - 1);
	res.push_back(tempStr);

	while (sep != std::string::npos)
	{
		while (_buffer[sep] == '\r' || _buffer[sep] == '\n')
			sep++;

		prev = sep;
		sep = _buffer.find("\r\n", prev);
		if (sep == std::string::npos)
			break ;
		tempStr = _buffer.substr(prev, sep - prev);

		if (tempStr.length() < 1)
			break;
		if (tempStr[tempStr.length() - 1] == ' ')
			tempStr = tempStr.substr(0, tempStr.length() - 1);
		res.push_back(tempStr);
	}

	return (res);
}

int Client::execCmd(Server &s, std::string &str)
{
	if (!_connected)
		setInfos(s, str);

	else
	{
		int cmd = getCmd(str);

		switch (cmd)
		{
			case USER:
			case PASS:
				sendToClient(_fd, ERR_ALREADYREGISTERED(_nickname));
				break;
			case NICK:
				nick(s, *this, str, false);
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
				break;
			case QUIT:
				quit(s, *this, str);
				return (1);
			case INVITE:
				invite(s, *this, str);
				break;
			case TOPIC:
				topic(s, *this, str);
				break;
			case MOTD:
				motd(s, *this);
				break;
			case PONG:
				pong(s, *this, str);
				break;
			case LIST:
				list(s, *this, str);
				break;
			case KICK:
				kick(s, *this, str);
				break;
			case AWAY:
				away(s, *this, str);
				break;
			case MODERATE:
				moderator(s, *this, str);
				break;
			case moderate:
				moderator(s, *this, str);
				break;
			default:
				std::size_t end = str.find(' ');
				std::string cmdStr = str.substr(0, end);
				sendToClient(this->getFd(), ERR_UNKNOWNCOMMAND(this->getNick(), cmdStr));
				break;
		}
	}
	return (0);
}

/**  Public member functions  **************************************************/

int	Client::read(Server &s) // Le serveur lit ce que lui envoit le client
{
	int r = recv(_fd, _bufRead, BUFFER_SIZE, 0);
	if (r <= 0)
	{
		s.removeClient(*this);
		return (1);
	}

	std::cout << YELLOW << "From " << _fd << ": -" << _bufRead << "-" << END << std::endl;
	std::string	test = _bufRead;

	_buffer += std::string(_bufRead, r);
	if (strstr(_buffer.c_str(), "\r\n") != NULL) {
		std::vector<std::string> cmds = splitBuf();

		for (std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it) {
			if (execCmd(s, *it) == 1) {
				return (0);
			}
		}
		std::size_t found = _buffer.rfind("\r\n");

		if (found != std::string::npos) {
			_buffer = _buffer.substr(found + 2);
		}
		else {
			_buffer.clear();
		}
	}
	std::memset(_bufRead, 0, BUFFER_SIZE);

	return (0);
}