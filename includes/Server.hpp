/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/08 13:29:20 by hgeffroy         ###   ########.fr       */
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

	time_t 					_creationTime;
	std::string				_name;
	std::string				_password;
	std::vector<Client> 	_clients; // Stocker des pointeurs !!
	std::vector<Channel>	_channels; // Stocker des pointeurs !!
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
	std::string 			getName() const;
	time_t* 				getCreationTime();

	int 	getClientFd(std::string nickname);
	void	delClient(int _fd);
	void	addChannel(Channel newChannel);

	void	initFd();
	void	checkFd();

};

#endif