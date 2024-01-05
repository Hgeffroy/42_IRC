/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:53:44 by twang             #+#    #+#             */
/*   Updated: 2024/01/05 11:05:17 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int	main( int ac, char **av )
{
	try
	{
		if ( ac != 5 )
			throw std::invalid_argument( "Wrong usage:\n./bot <port> <password> <apikey>");
		Bot	bot( av[1], av[2], av[3] );
	}
	catch ( std::invalid_argument const & error )
	{
		std::cerr << error.what( ) << std::endl;
		return ( -1 );
	}

	return ( 0 );
}
