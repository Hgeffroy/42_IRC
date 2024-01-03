/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:44:16 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/20 16:29:51 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	nick(Server& s, Client& c, std::string& str) // Verifier la taille
{
	if (str.find(' ', 5) != std::string::npos)
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "NICK", "Too many parameters"));
		return ;
	}

	size_t 		nextSpace = str.find_first_of("\n\r", 5);
	std::string nick;
	if (nextSpace == std::string::npos && str.size() > 5)
		nick = str.substr(5);
	else if (nextSpace != std::string::npos)
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
		prev = c.getNick();
	}

	c.setNick(nick); // Whitespaces ?

	if (!prev.empty())
	{
		sendToClient(c.getFd(), SWITCH_NICK(prev, nick));
	}
}