/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:52:42 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/03 13:28:42 by twang            ###   ########.fr       */
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