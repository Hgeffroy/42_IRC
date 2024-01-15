/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:47:13 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:12:51 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	pass(Server& s, Client& c, std::string& str)
{
	if (c.getConnected())
	{
		sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
		return ;
	}

	size_t		nextSpace = str.find_first_of("\r ", 5);
	std::string	pass;
	if (nextSpace == std::string::npos && str.size() > 5)
		pass = str.substr(5);
	else if (nextSpace != std::string::npos)
		pass = str.substr(5, nextSpace - 5);

	if (pass.empty())
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PASS"));
		return ;
	}

	if (pass == s.getPass())
		c.setPassOk();
	else {
		sendToClient(c.getFd(), ERR_PASSWDMISMATCH(c.getNick()));
		c.setPassNotOk();
	}
}
