/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:44:16 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:43:58 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	nick(Server& s, Client& c, std::string& str, bool is_bot )
{
	std::string	nick;
	size_t		nextSpace = str.find_first_of("\r ", 5);
	if (nextSpace == std::string::npos)
		nextSpace = str.find(' ', 5);
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
	if (nick.size() > 10)
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "NICK", "Nickname too long(max size 10)"));
		return ;
	}
	if (nick == "bot" && !is_bot)
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "NICK", "This nickname is already reserved"));
		return ;
	}

	std::string	prev;
	if (!c.getNick().empty())
	{
		prev = c.getNick();
	}

	c.setNick(s, nick);

	if (!prev.empty())
	{
		sendToClient(c.getFd(), SWITCH_NICK(prev, nick));
	}
}
