/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:30:24 by twang             #+#    #+#             */
/*   Updated: 2024/01/15 10:27:51 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_CPP
# define BOT_CPP

/*---- includes ---------------------------------------------------------------*/

# include "bot.h"

/*---- class defines ----------------------------------------------------------*/

class Bot
{

	private:

		std::size_t	_socket;
		std::string	_password;
		std::string	_apiKey;
		char		_bufRead[BUFFER_SIZE];

		static std::string	setPassword( std::string& password );
		static std::size_t	setPort( std::string& port );
		void				connect( int port, std::string& password );

	public:

		Bot( std::string port, std::string password, std::string apikey );
		~Bot( void );

		bool						checkApiKey( void );
		void						sendToServer( std::string str );
		int							readFromServer( void );
		int							execute( std::string &buffer );
		void						privmsg( std::string &msg, std::string &usr );
		void						moderate( std::string &msg, std::string &usr );

		std::vector< std::string >	splitBuffer( void );

};

/*---- structures -------------------------------------------------------------*/

typedef struct s_commands	t_commands;

struct s_commands
{
	std::string	key;
	void		(Bot::*function)( std::string &str, std::string &usr );
};

/*---- prototypes -------------------------------------------------------------*/

std::string					splitCommand( std::string &buffer );
std::string					splitMessage( std::string &buffer );
std::vector< std::string >	splitArguments( std::string &s );

#endif