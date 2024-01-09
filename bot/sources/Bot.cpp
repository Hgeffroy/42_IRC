/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wangthea <wangthea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:33:13 by twang             #+#    #+#             */
/*   Updated: 2024/01/08 22:07:29 by wangthea         ###   ########.fr       */
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
	_apiKey = apikey;
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
	sendToServer( "JOIN #bot\n" );
	while (true)
		readFromServer();
}

/**  Public member functions  *****************************************************************************************/

void	Bot::sendToServer( std::string str )
{
	send( _socket, str.c_str(), str.length(), MSG_DONTWAIT + MSG_NOSIGNAL);
	std::cout << BLUE << "To " << _socket << ": " << str << END;
}

int	Bot::readFromServer( void )
{
	int r = recv( _socket, _bufRead, BUFFER_SIZE, 0);

	if ( r <= 0 )
		return (-1);

	std::cout << RED << "From " << _socket << ": " << _bufRead << END;

	std::vector<std::string> cmds = splitBuffer();
	for ( std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++ )
		if ( execute( *it ) == 1 )
			return ( 0 );

	std::memset( _bufRead, 0, BUFFER_SIZE );

	return (0);
}

int	Bot::execute( std::string &buffer )
{
	const t_commands	list[] = { {"PRIVMSG", &Bot::privmsg }, {"MODERATE", &Bot::moderate} };
	std::string			command = splitCommand( buffer );
	std::string			user;
	if (buffer.find('#', 0) == std::string::npos)
		user = buffer.substr(1, buffer.find(' '));
	else
		user = buffer.substr(buffer.find('#', 0), buffer.find(' ', buffer.find('#', 0)) - buffer.find('#', 0)) + " " + buffer.substr(1, buffer.find(' '));
	std::string			msg = splitMessage( buffer );

	for ( int i = 0; i < 2; i++ )
		if ( command == list[i].key )
			( this->*list[i].function )( msg, user );

	return (0);
}

std::string	getGPTanswer(const char *str)
{
	FILE*	pipe = popen(str, "r");
	if (!pipe) {
		std::cerr << RED << "Error popen()" << END << std::endl;
		return ("Error when using popen function");
	}

	char	buff[256];
	std::string res;

	while (fgets(buff, 256, pipe) != NULL) {
		if (ferror(pipe)) {
			std::cerr << RED << "Error fgets()" << END << std::endl;
			return ("Error when using fgets function");
		}
		res += buff;
	}
	pclose(pipe);
	return (res);
}

void	Bot::privmsg( std::string &msg, std::string &usr )
{
	if (usr[0] != '#') {
		std::string command = ASSISTANT;
		std::string answer = getGPTanswer(command.c_str());
		sendToServer( "PRIVMSG " + usr + " " + answer + "\n" );
	}
	else {
		std::string command = MODERATOR;
		std::string answer = getGPTanswer(command.c_str());
		if (answer == "\"KICK\"\n") {
			std::string channel = usr.substr(0, usr.find(' '));
			std::string toBeKicked = usr.substr(usr.find(' '), usr.length() - usr.find(' '));
			sendToServer( "KICK " + channel + toBeKicked + " : Bad words in the chat\n" );
		}
	}
}

void	Bot::moderate( std::string &msg, std::string &usr )
{
	( void )usr;
	std::vector< std::string >	channels = splitArguments( msg );
	if ( channels.empty() )
		return ;
	for ( std::vector< std::string >::iterator		it = channels.begin() ; it != channels.end(); it++ )
		sendToServer( "JOIN " + *it + "\n" );
}
