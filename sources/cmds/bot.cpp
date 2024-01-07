/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:04:56 by twang             #+#    #+#             */
/*   Updated: 2024/01/07 11:19:28 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	bot( Server &s, Client& c, std::string& str )
{
	( void )str;

	std::string userCmd = "USER bot\n";
	std::string nickCmd = "NICK bot\n";
	std::string joinCmd = "JOIN #bot";

	user( s, c, userCmd, true );
	nick( s, c, nickCmd, true );
}
