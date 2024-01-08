/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:33:13 by twang             #+#    #+#             */
/*   Updated: 2024/01/07 15:02:15 by twang            ###   ########.fr       */
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
	sendToServer( "JOIN #bot\n" );
	// sendToServer( "LIST\n" );
	while (true)
	{
		readFromServer();
	}

	// 322
	// envoie LIST, parse list puis join un par un.
	// status de operator.
}

/**  Public member functions  *****************************************************************************************/

void	Bot::sendToServer( std::string str )
{
	send( _socket, str.c_str(), str.length(), MSG_DONTWAIT + MSG_NOSIGNAL);
	std::cout << BLUE << "To " << _socket << ": " << str << END;
}

int	Bot::readFromServer( void ) // Le bot lit ce que lui renvoit le client
{
	int r = recv( _socket, _bufRead, BUFFER_SIZE, 0);

	if ( r <= 0 )
		return (-1);

	std::cout << RED << "From " << _socket << ": " << _bufRead << END << std::endl;

	std::vector<std::string> cmds = splitBuffer();
	for ( std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); it++ )
		if ( execute( *it ) == 1 )
			return ( 0 );

	std::memset( _bufRead, 0, BUFFER_SIZE );

	return (0);
}

int	Bot::execute( std::string &buffer )
{
	const t_commands	list[] = { {"PRIVMSG", &Bot::privmsg } };
	std::string			command = splitCommand( buffer );
	std::string			user = buffer.substr(1, buffer.find(' '));
	std::string			msg = splitMessage( buffer );

	for ( int i = 0; i < 1; i++ )
		if ( command == list[i].key )
			( this->*list[i].function )( msg ); // I NEED USER IN THE ARGS !!!!!!

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

void	Bot::privmsg( std::string &msg )
{
	std::string apiKey = "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";
	std::cout << PURPLE << msg << END << std::endl;
	std::string request = "curl -s https://api.openai.com/v1/chat/completions \
	-H \"Content-Type: application/json\" \
	-H \"Authorization: Bearer " + _apiKey + "\" \
	-d '{ \
		\"model\": \"gpt-3.5-turbo-16k\", \
		\"messages\": [ \
			{ \
				\"role\": \"system\", \
				\"content\": \"You are a helpful assistant that can give answers with 500 caracters at most.\"" // A voir si il faut changer ici ou po.
			+ "}, \
			{ \
				\"role\": \"user\", \
				\"content\": " + msg + "\" \
			} \
		] \
	}' | jq '.choices[].message.content'";
	//std::cout << request << std::endl;
	std::string answer = getGPTanswer(request.c_str());
	std::cout << "-" << answer << "-" << std::endl;
}
