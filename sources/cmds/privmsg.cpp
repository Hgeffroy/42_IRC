/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:54:50 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/14 14:13:37 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendDM(Server& s, Client& c, std::string& dest, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();
	std::cout << c.getNick() << " tries to send a msg to client " << dest << std::endl;

	std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + dest + " :" + msg + ENDLINE;
	::sendToClient(clients[dest]->getFd(), fullMsg);
	if (clients[dest]->getAway())
		::sendToClient(c.getFd(), RPL_AWAY(c.getNick(), clients[dest]->getNick())); // Le RPL away ne permet pour l'instant pas de set le message away

	// ::sendToClient(c.getFd(), ERR_NOSUCHNICK(c.getNick(), dest));
}

void	sendChan(Server& s, Client& c, std::string& dest, std::string& msg)
{
	std::map<std::string, Channel*>	channels = s.getChannels();
	std::cout << c.getNick() << " tries to send a msg to channel " << dest << std::endl;

	if ((channels[dest])->getName() == dest)
	{
		std::map<std::string, std::string>	members = (channels[dest])->getMembers();
		for (std::map<std::string, std::string>::iterator it2 = members.begin(); it2 != members.end(); ++it2)
		{
			if (it2->first != c.getNick())
			{
				std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + dest + " " + msg + ENDLINE; // Ce msg est pas bon
				::sendToClient(s.getClientFd(it2->first), fullMsg);
			}
		}
		return ;
	}
	::sendToClient(c.getFd(), ERR_CANNOTSENDTOCHAN(c.getNick(), dest));
}

void	sendBroadcast(Server& s, Client& c, std::string& msg)
{
	std::map<std::string, Client*>	clients = s.getClients();
	std::cout << c.getNick() << " tries to send a msg in broadcast" << std::endl;
	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) // Send to one client
	{
		std::string	fullMsg = ":" + c.getNick() + " PRIVMSG " + it->first+ " :" + msg + ENDLINE;
		::sendToClient(it->second->getFd(), msg);
	}
}

void	sendMsg(Server& s, Client& c, std::string& str)
{
	int sep1 = static_cast<int>(str.find(' '));
	int sep2 = static_cast<int>(str.find(' ', sep1 + 1));

	// Mettre des protections ici !

	std::string	dest = str.substr(sep1 + 1, sep2 - sep1 - 1);
	std::string msg = str.substr(sep2 + 1);

	if (msg.empty())
	{
		::sendToClient(c.getFd(), ERR_NOTEXTTOSEND(c.getNick()));
		return ;
	}

	if (std::isalpha(dest[0]))
		sendDM(s, c, dest, msg);
	else if (dest[0] == '#')
		sendChan(s, c, dest, msg);
	else if (dest[0] == '$')
		; // Send to all clients on server (broadcast)
}