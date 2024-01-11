/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:52:42 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/11 16:23:37 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* Enlever le client de tous les channels
 * Enlever le client du server */
void	quit(Server& s, Client& c, std::string& str)
{
	(void)str;
	/*doit renvoyer une RPL comme dans part, mais il faudrait recoder (?)
	removeUserFromChan(), et ne pas avoir le delete channel a l'interieur
	soit tu le l'atteins pas, soit tu segfault*/
	s.removeClient(c);
}