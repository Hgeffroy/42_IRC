/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 08:42:50 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.h"

class	Server
{
private:

	std::string			_password;
	std::vector<Client> _clients;
	fd_set				_fdWrite; // Ceux a qui le serveur va ecrire
	fd_set				_fdRead; // Ceux que le serveur doit lire

	Server();
	static int			setPort(std::string& portstr);
	static std::string	setPassword(std::string& pass);
	void				accept(Client& client);
	int 				higherFd() const;

public:

	~Server();
	Server(std::string port, std::string password);

	void	initFd();
	void	checkFd();

};

#endif