/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:48:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/26 17:24:15 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**  Constructors and destructors  ************************************************************************************/

Server::Server()
{

}

Server::~Server()
{

}

Server::Server(std::string port, std::string password)
{
	_port = checkPort(port);
	_password = checkPassword(password);
}

/**  Private member functions  ****************************************************************************************/

int	Server::checkPort(std::string portstr)
{
	char* end;
	int	port = static_cast<int>(std::strtol(portstr.c_str(), &end, 10));
	if (errno == ERANGE || port == 0 || *end)
		throw std::invalid_argument("<port> is not an integer");
	if (port < 1024 || port > 65535)
		throw std::invalid_argument("<port> is not a valid port");
	return (port);
}

std::string	Server::checkPassword(std::string pass)
{
	if (pass.size() < 5)
		throw std::invalid_argument("<password> is too short");


	std::string::iterator	it;
	for (it = pass.begin(); it != pass.end(); ++it)
		if (!std::isprint(*it) || *it == ' ' || *it == '	')
			throw std::invalid_argument("<password> has an invalid character");

	return (pass);
}
