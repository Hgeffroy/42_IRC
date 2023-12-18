/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:00:29 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 15:12:43 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	pong(Server& s, Client& c, std::string str)
{
	(void)s;
	std::string	token;
	size_t	space = str.find(' ');

	token = str.substr(space + 1);
	sendToClient(c.getFd(), PONG_MSG(token));
}