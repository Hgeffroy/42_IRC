/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:44:37 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/30 08:36:50 by hgeffroy         ###   ########.fr       */
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
	std::string	_bufRead;
	std::string	_bufWrite;

	Client();

public:

	explicit	Client(int type, int socket);
				~Client();

	int 		getType() const;
	std::string	getBufRead() const;
	std::string	getBufWrite() const;
	int 		getFd() const;
	void		setType(int newType);

	void	write();
	void	read();
//	void	(*fct_read)();
//	void	(*fct_write)();

};

#endif