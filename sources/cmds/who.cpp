/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 08:57:14 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 09:28:04 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	who(Server& s, Client& c, std::string& str)
{
	int 		space = static_cast<int>(str.find(' '));
	std::string mask = str.substr(space + 1, str.length() - (space + 1));
	std::cout << mask << std::endl;

	if (mask[0] == '#')
	{

		// A envoyer pour tous les membres du chan
		sendToClient(c.getFd(), RPL_WHOREPLY(c.getNick(), mask, c.getUser(), getIP(), c.getNick(), "H@", '0', c.getUser())); // H if user here, G if gone, @ for ops (second thing on the map in chan)
		// mask of RPL_ENDOFWHO is the mask name
		sendToClient(c.getFd(), RPL_ENDOFWHO(c.getNick(), mask));
	}
}