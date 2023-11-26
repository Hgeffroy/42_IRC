/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/26 17:01:45 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"

class	Server
{
private:

	int			_port;
	std::string	_password;
	
	Server();
	int			checkPort(std::string portstr);
	std::string	checkPassword(std::string pass);
	
public:

	~Server();
	Server(std::string port, std::string password);
	
};

#endif