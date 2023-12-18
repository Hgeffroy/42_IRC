/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:44:16 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 14:25:50 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	nick(Server& s, Client& c, std::string& str) // Verifier la taille
{
	int 		nextSpace = static_cast<int>(str.find_first_of(" \n\r", 6));
	std::string nick;
	if (nextSpace == static_cast<int>(std::string::npos))
		nick = str.substr(5);
	else
		nick = str.substr(5, nextSpace - 5);

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNick() == nick)
		{
			sendToClient(c.getFd(), ERR_NICKNAMEINUSE(c.getNick(), nick));
			return ;
		}
	}

	if (nick.empty())
	{
		sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return ;
	}

	std::string	prev;
	if (!c.getNick().empty())
	{
		std::cout << "ici" << std::endl;
		prev = c.getNick();
	}

	c.setNick(nick); // Whitespaces ?

	if (!prev.empty())
	{
		std::cout << "la" << std::endl;
		sendToClient(c.getFd(), SWITCH_NICK(prev, nick));
	}

//	std::cout << "Nickname set to: " << c.getNick() << std::endl;
}