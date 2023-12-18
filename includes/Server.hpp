/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 12:14:06 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.h"
# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class	Server
{

private:

	int								_listener;
	time_t 							_creationTime;
	std::string						_name;
	std::string						_password;
	std::vector<Client*>			_newClients;
	std::map<std::string, Client*> 	_clients;
	std::map<std::string, Channel*>	_channels;
	fd_set							_fdRead; // Ceux que le serveur doit lire

	Server();

	static int			setPort(std::string& portstr);
	static std::string	setPassword(std::string& pass);

	void				accept();
	int 				higherFd() const;
	void				removeClientFromServers(Client& c);

public:

	~Server();
	Server(std::string port, std::string password);

	std::map<std::string, Client*>	getClients() const;
	std::map<std::string, Channel*>	getChannels() const;
	std::vector<Client*>			getNewClients() const;
	std::string						getPass() const;
	std::string 					getName() const;
	time_t* 						getCreationTime();

	int 	getClientFd(std::string nickname);

	void	addClient(Client* client);
	void	removeClient(Client& c);
	void	addChannel(Channel* newChannel);

	void	initFd();
	void	checkFd();

};

#endif