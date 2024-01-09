/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 12:32:41 by twang             #+#    #+#             */
/*   Updated: 2024/01/09 12:59:06 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	signals_switchman( int signal )
{
	if (signal == SIGINT)
		throw std::runtime_error( "catch signal : SIGINT" );
}
