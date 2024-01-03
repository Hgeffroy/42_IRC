/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:00:21 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/03 15:36:56 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	user(Server& s, Client& c, std::string& str)
{
	size_t 		nextSpace;
	if (str.find(' ', 5) != std::string::npos)
		nextSpace = str.find(' ', 5);
	else
		nextSpace = str.find_first_of("\n\r", 5);

	std::string usr;

	if (nextSpace == std::string::npos && str.size() > 5)
		usr = str.substr(5);
	else if (nextSpace != std::string::npos)
		usr = str.substr(5, nextSpace - 5);

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getUser() == usr)
		{
			sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
			return ;
		}
	}

	if (usr.empty())
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "USER"));
		return ;
	}

	c.setUser(usr);
	std::cout << "Username set to: " << c.getUser() << std::endl;
}