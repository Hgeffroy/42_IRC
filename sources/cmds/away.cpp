/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 09:21:07 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/07 09:33:54 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::string	getReason(std::string& str);

/*----------------------------------------------------------------------------*/

void	away(Server& s, Client& c, std::string& str)
{
	(void)s;
	std::string	reason = getReason(str);
	if (reason.empty() && c.getAway()) {
		c.setAway(false);
		sendToClient(c.getFd(), RPL_UNAWAY(c.getNick()));
	}
	else if (!reason.empty() && !c.getAway()) {
		c.setAway(true);
		sendToClient(c.getFd(), RPL_NOWAWAY(c.getNick()));
	}
}

static std::string	getReason(std::string& str)
{
	size_t space = str.find(' ');
	if (space == std::string::npos)
		return ("");
	return (str.substr(space + 1));
}