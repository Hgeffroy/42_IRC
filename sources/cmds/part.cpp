/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 10:05:57 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/15 10:58:38 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	part(Server& s, Client& c, std::string& str)
{
	std::size_t			space1 = str.find(' ');
	std::size_t 		space2 = str.find(' ', space1 + 1);
	std::string	chan = str.substr(space1 + 1, space2 - space1 - 1);
	std::cout << "Chan in part is: " << chan  << "END" << std::endl;

	std::map<std::string, Channel*>				channels = s.getChannels();
	std::map<std::string, Channel*>::iterator 	itChan = channels.find(chan);

	if (itChan == channels.end())
	{
		std::cout << "No such chan" << std::endl;
		return ; // ERR_NOSUCHCHANNEL (403)
	}

	std::map<std::string, std::string>				members = itChan->second->getMembers();
	std::map<std::string, std::string>::iterator	itMembers = members.find(c.getNick());

	if (itMembers == members.end())
	{
		std::cout << "Not in channel" << std::endl;
		return ; // ERR_NOTONCHANNEL (442)
	}

	members.erase(itMembers->first);
}