/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:47:13 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/12 08:50:37 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	pass(Server& s, Client& c, std::string& str)
{
	if (c.getConnected())
	{
		::sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
		return ;
	}

	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string pass;
	if (nextSpace == static_cast<int>(std::string::npos))
		pass = str.substr(5);
	else
		pass = str.substr(5, nextSpace - 5);

	std::cout << "Pass is: " << pass << " of size " << pass.size() << std::endl;

	if (pass.empty())
	{
		::sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PASS"));
		return ;
	}

	if (pass == s.getPass())
	{
		c.setPassOk();
		std::cout << "Correct password" << std::endl;
	}
	else
		::sendToClient(c.getFd(), ERR_PASSWDMISMATCH(c.getNick())); // May close connection
}