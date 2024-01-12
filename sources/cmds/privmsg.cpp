/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:54:50 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/12 14:56:19 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendDM(Server& s, Client& c, std::string& dest, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();

	if (clients.find(dest) == clients.end())
	{
		sendToClient(c.getFd(), ERR_NOSUCHNICK(c.getNick(), dest));
		return ; // Err a send
	}

	std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + dest + " :" + msg + ENDLINE;
	sendToClient(clients[dest]->getFd(), fullMsg);
	if (clients[dest]->getAway())
		sendToClient(c.getFd(), RPL_AWAY(c.getNick(), clients[dest]->getNick())); // Le RPL away ne permet pour l'instant pas de set le message away
}

void	sendChan(Server& s, Client& c, std::string& dest, std::string& msg)
{
	std::map<std::string, Channel*>	channels = s.getChannels();

	if (channels.find(dest) == channels.end())
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), dest));
		return ; // Err a send
	}

	std::map<std::string, std::string>	members = channels[dest]->getMembers();
	if (members.find(c.getNick()) == members.end()) {
		sendToClient(c.getFd(), ERR_USERNOTINCHANNEL(c.getNick(), c.getNick(), (*channels[dest]).getName()));
		return ;
	}
	for (std::map<std::string, std::string>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
	{
		if (it2->first != c.getNick())
		{
			std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + dest + " " + msg + ENDLINE;
			sendToClient(s.getClientFd(it2->first), fullMsg);
		}
	}
}

void	sendBroadcast(Server& s, Client& c, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();
	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) // Send to one client
	{
		std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + it->first + " :" + msg + ENDLINE;
		sendToClient(it->second->getFd(), msg);
	}
}

void	sendMsg(Server& s, Client& c, std::string& str)
{
	size_t sep1 = str.find(' ');
	size_t sep2 = str.find(' ', sep1 + 1);
	if (sep1 == std::string::npos || sep2 == std::string::npos)
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PRIVMSG"));
		return ;
	}

	// Mettre des protections ici !

	std::string	dest = str.substr(sep1 + 1, sep2 - sep1 - 1);
	std::string msg = str.substr(sep2 + 1);

	if (msg.empty())
	{
		sendToClient(c.getFd(), ERR_NOTEXTTOSEND(c.getNick()));
		return ;
	}

	if (std::isalpha(dest[0]))
		sendDM(s, c, dest, msg);
	else if (dest[0] == '#' || dest[0] == '&')
		sendChan(s, c, dest, msg);
	else if (dest[0] == '$')
		sendBroadcast(s, c, msg); // Send to all clients on server (broadcast)
}