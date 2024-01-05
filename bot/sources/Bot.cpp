/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:33:13 by twang             #+#    #+#             */
/*   Updated: 2024/01/05 16:27:26 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

/**  Constructors and destructors  ************************************************************************************/

Bot::Bot( void ) {}

Bot::Bot( std::string port, std::string password, std::string apikey )
{
	struct sockaddr_in	sInfo;
	int					p;
	int					s;
	int					optval = 1;

	close( STDIN_FILENO );
	p = setPort( port );
	_password = setPassword( password );
	s = socket( PF_INET, SOCK_STREAM, 0 );
	if ( s < 0 )
		throw std::runtime_error( "socket failed" );
	setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof( optval ) );
	sInfo.sin_family = AF_INET;
	sInfo.sin_addr.s_addr = INADDR_ANY;
	sInfo.sin_port = htons( 2001 );
	if ( bind( s, reinterpret_cast< sockaddr* >( &sInfo ), sizeof( sInfo ) ) < 0 )
		throw std::runtime_error( "bind failed" );
	_socket = s;
	connect( p, _password );
}

Bot::~Bot( void ) {}

/**  Setters and Getters  *********************************************************************************************/

/**  Private member functions  ****************************************************************************************/

std::string	Bot::setPassword( std::string& password )
{
	if ( password.size( ) < 5 )
		throw std::invalid_argument( "<password> is too short" );

	for ( std::string::iterator it = password.begin( ); it != password.end( ); ++it )
	{
		if ( !std::isprint( *it ) || *it == ' ' || *it == '	' )
			throw std::invalid_argument( "<password> has an invalid character" );
	}

	return ( password );
}

std::size_t	Bot::setPort( std::string& port )
{
	char*		end;
	std::size_t	new_port = static_cast<int>( std::strtol( port.c_str(  ), &end, 10 ) );

	if ( errno == ERANGE || new_port == 0 || *end )
		throw std::invalid_argument( "<port> is not an integer" );

	if ( new_port < 1024 || new_port > 65535 )
		throw std::invalid_argument( "<port> is not a valid port" );

	return ( new_port );
}

void	Bot::connect( int port, std::string& password )
{
	struct sockaddr_in	csin;

	csin.sin_family = AF_INET;
	csin.sin_port = htons( port );
	if ( inet_pton( AF_INET, "127.0.0.1", &csin.sin_addr ) <= 0 )
		throw std::invalid_argument( "<port> is not a valid port" );
	if ( ::connect( _socket, reinterpret_cast< const struct sockaddr* >( &csin ), sizeof( csin ) ) < 0 )
		throw std::invalid_argument( "<connect> bot cannot connect to the server" );

	sendToServer( "PASS " + password + "\n" );
	sendToServer( "BOT\n" );
	// sendToServer( "LIST\n" );

	// peut pas join #bot si tu ne t'apelle pas bot
	// envoie LIST, parse list puis join un par un.
	// status de operator.
}

/**  Public member functions  *****************************************************************************************/

void	Bot::sendToServer( std::string str )
{
	send( _socket, str.c_str(), str.length(), MSG_DONTWAIT + MSG_NOSIGNAL);
	std::cout << BLUE << "To " << _socket << ": " << str << END;
}