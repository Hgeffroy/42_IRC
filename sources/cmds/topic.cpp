/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:42 by twang             #+#    #+#             */
/*   Updated: 2024/01/15 10:00:43 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	topic(Server& s, Client& c, std::string& str)
{
	std::string	target;

	int start = str.find(' ');
	int end = str.find(' ', start + 1);
	if (end == -1)
		end = str.size();
	target = str.substr(start + 1, end - start - 1);
	std::map<std::string, Channel*>	chan = s.getChannels();
	if (!chan[target]) {
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), target));
		return ;
	}
	std::map<std::string, std::string> members = (*chan[target]).getMembers();
	if (str.size() - end == 0) {
		if (members.find(c.getNick()) == members.end()) {
			sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), (*chan[target]).getName()));
			return;
		}
		if (chan[target]->getTopic().empty()) {
			sendToClient(c.getFd(), RPL_NOTOPIC(c.getNick(), target));
		}
		else {
			sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), target, chan[target]->getTopic()));
		}
		return;
	}
	size_t i;
	for (i = end; i < str.size(); i++)
	{
		if (str[i] == ' ' || str[i] == ':')
			continue;
		else
			break;
	}
	end = i;
	std::string modeStr = str.substr(end, str.size() - (end));
	if (chan[target]->getTopicProtect() == true) {
		if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
			sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), (*chan[target]).getName()));
			return ;
		}
	}
	chan[target]->setTopic(modeStr);
	std::map<std::string, Client*>	clients = s.getClients();
	for (std::map<std::string, std::string>::iterator it = members.begin(); it != members.end(); ++it)
	{
		sendToClient(clients[it->first]->getFd(), RPL_TOPIC(it->first, target, chan[target]->getTopic()));
	}
}
