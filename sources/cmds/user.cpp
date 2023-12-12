/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:00:21 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/12 09:26:12 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	user(Server& s, Client& c, std::string& str) // Doublon ?
{
	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string usr;

	if (nextSpace == static_cast<int>(std::string::npos))
		usr = str.substr(5);
	else
		usr = str.substr(5, nextSpace - 5);

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getUser() == usr)
		{
			::sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
			return ;
		}
	}

	if (usr.empty())
	{
		::sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "USER"));
		return ;
	}

	c.setUser(usr);
	std::cout << "Username set to: " << c.getUser() << std::endl;
}