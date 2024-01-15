/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:00:21 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 13:07:46 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	user(Server& s, Client& c, std::string& str, bool is_bot )
{
	std::string	usr;

	std::size_t	nextSpace = str.find(' ', 5);
	if (nextSpace != std::string::npos)
		usr = str.substr(5, nextSpace - 5);
	else {
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "USER"));
		return ;
	}

	int	i = 1;
	while (nextSpace != std::string::npos) {
		nextSpace = str.find_first_of("\r ", nextSpace + 1);
		i++;
	}

	if (i < 4) {
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "USER"));
		return ;
	}

	std::map<std::string, Client*>	clients = s.getClients();

	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getUser() == usr)
		{
			sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
			return ;
		}
	}
	// TODO pas de secu de ca. le seul identifiant unique est le nickName
			// if (it->second->getUser() == usr)
			// {
			//		sendToClient(c.getFd(), ERR_ALREADYREGISTERED(c.getNick()));
			//		return ;
			// }
	if (usr.empty())
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "USER"));
		return ;
	}

	if (usr == "bot" && !is_bot)
	{
		sendToClient(c.getFd(), ERR_UNKNOWNERROR(c.getNick(), "NICK", "This username is already reserved"));
		return ;
	}

	c.setUser(usr);
}
