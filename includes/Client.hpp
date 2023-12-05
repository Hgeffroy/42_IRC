/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:44:37 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 15:18:03 by hgeffroy         ###   ########.fr       */
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

	int							_type;
	int 						_fd;
	bool						_connected;
	bool						_passwordOk;
	std::string 				_nickname; // Imposer de commencer par un alpha !
	std::string					_username; // Imposer de commencer par un alpha !
	char 						_bufRead[BUFFER_SIZE]; // Ce que lit le serveur chez le client
	char 						_bufWrite[BUFFER_SIZE]; // Ce que write le serveur au client

	int		setInfos(std::string serverPass);
	int 	getCmd(std::string buffer);
	void	setPass(std::string& s, std::string& serverPass);
	void	setNick(std::string s);
	void	setUser(std::string s);
	void	sendMsg(std::vector<Client>& c);
	void	join(Server& s);

public:

	Client(int type, int socket);
	~Client();

	int 			getType() const;
	const char*		getBufRead() const;
	const char*		getBufWrite() const;
	int 			getFd() const;
	void			setType(int newType);

	void	write();
	void	read(Server& s);

};

#endif