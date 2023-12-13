/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/13 12:20:08 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

//i: Set/remove Invite-only channel
//k: Set/remove the channel key (password)

void	i_opt(std::vector< Channel* >& channel, Client& c, std::string& params)
{
	std::cout << channel << std::endl;
	std::cout << params << std::endl;
}

// void	k_opt(std::vector< Channel* >& channel, Client& c, std::string& params)
// {
	
// }
