/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:44:37 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 16:09:10 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.h"

class	Client
{
private:

	int			_type;
	int 		_fd;
	bool		_connected;
	bool		_passwordOk;
	std::string _nickname;
	std::string	_username;
	char 		_bufRead[BUFFER_SIZE]; // Ce que lit le serveur chez le client
	char 		_bufWrite[BUFFER_SIZE]; // Ce que write le serveur au client

	int	setInfos(std::string pass);

public:

	Client(int type, int socket);
	~Client();

	int 			getType() const;
	const char*		getBufRead() const;
	const char*		getBufWrite() const;
	int 			getFd() const;
	void			setType(int newType);

	void	write();
	void	read(std::vector<Client>& clients, std::string pass);

};

#endif