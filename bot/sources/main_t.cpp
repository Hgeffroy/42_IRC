/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:53:44 by twang             #+#    #+#             */
/*   Updated: 2024/01/04 14:34:44 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.h"

// ./bot server port password

int	main( int ac, char **av )
{
	if ( ac != 4 )
	{
		std::cout << "Wrong usage:" << std::endl;
		std::cout << "./bot <server> <port> <password>" << std::endl;
		return (-1);
	}

	return ( 0 );
}
