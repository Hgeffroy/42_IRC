/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:00:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:00:07 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	pong(Server& s, Client& c, std::string str)
{
	(void)s;
	std::string	token;
	size_t	space = str.find(' ');

	if (space == std::string::npos)
	{
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "PONG"));
		return ;
	}

	token = str.substr(space + 1);
	sendToClient(c.getFd(), PONG_MSG(token));
}
