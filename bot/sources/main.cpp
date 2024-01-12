/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:53:44 by twang             #+#    #+#             */
/*   Updated: 2024/01/12 10:20:37 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

/*---- static defines --------------------------------------------------------*/

static void	handle_signals( int signal );

/*----------------------------------------------------------------------------*/

int	main( int ac, char **av )
{
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, &handle_signals);

	try
	{
		if ( ac != 4 )
			throw std::invalid_argument( "Wrong usage:\n./bot <port> <password> <apikey>");
		Bot	bot( av[1], av[2], av[3] );
	}
	catch ( std::exception const & error )
	{
		std::cerr << "Bot closed: ";
		std::cerr << error.what( ) << std::endl;
		return ( -1 );
	}
	return ( 0 );
}

static void	handle_signals( int signal )
{
	if ( signal == SIGINT )
		throw std::runtime_error( "catch signal : SIGINT" );
}
