/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:52:42 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 11:26:36 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* Enlever le client de tous les channels
 * Enlever le client du server */
void	quit(Server& s, Client& c, std::string& str)
{
	(void)str;
	s.removeClient(c);
}