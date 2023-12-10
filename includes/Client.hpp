/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:44:37 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/10 09:22:53 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.h"
# include "Server.hpp"

class Server;

class	Client
{

private:

	int 						_fd;
	bool						_connected;
	bool						_passwordOk;
	bool						_away;
	std::string 				_nickname; // Imposer de commencer par un alpha !
	std::string					_username; // Imposer de commencer par un alpha !
	char 						_bufRead[BUFFER_SIZE]; // Ce que lit le serveur chez le client
	char 						_bufWrite[BUFFER_SIZE]; // Ce que write le serveur au client

	int 						getCmd(std::string& buffer);
	void						execCmd(Server &s, std::string& str);
	std::vector<std::string>	splitBuf();

	void	setNick(std::string str, Server& s);
	void	setUser(std::string str, Server& s);
	void	setPass(std::string& str, Server& s);
	int		setInfos(Server& s, std::string& str);

	void	sendDM(Server& s, std::string& dest, std::string& msg);
	void	sendChan(Server& s, std::string& dest, std::string& msg);
	void	sendBroadcast(Server& s, std::string& msg);
	void	sendMsg(Server& s, std::string& str);

	void	join(Server& s, std::string& str);

public:

	Client(int socket);
	~Client();

	int 			getType() const;
	int 			getFd() const;
	const char*		getBufWrite() const;
	std::string 	getNick() const;
	std::string 	getUser() const;

	void	sendToClient(int fd, std::string str) const;
	void	read(Server& s);

};

#endif