/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 14:21:17 by hgeffroy         ###   ########.fr       */
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

	std::string				_password;
	std::vector<Client> 	_clients;
	std::vector<Channel>	_channels;
	fd_set					_fdWrite; // Ceux a qui le serveur va ecrire
	fd_set					_fdRead; // Ceux que le serveur doit lire

	Server();

	static int			setPort(std::string& portstr);
	static std::string	setPassword(std::string& pass);

	void				accept(Client& client);
	int 				higherFd() const;

public:

	~Server();
	Server(std::string port, std::string password);

	std::vector<Client>&	getClients(); // Volontairement pas const, je veux bien la ref de ce truc pas une copie !!!
	std::vector<Channel>&	getChannels(); // Volontairement pas const, je veux bien la ref de ce truc pas une copie !!!
	std::string				getPass() const;

	void	delClient(int _fd);
	void	addChannel(Channel newChannel);

	void	initFd();
	void	checkFd();

};

#endif