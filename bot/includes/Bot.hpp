/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:30:24 by twang             #+#    #+#             */
/*   Updated: 2024/01/08 13:58:57 by twang            ###   ########.fr       */
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

		// std::string	_addr; -> voir avec arthur?
		std::size_t	_socket;
		std::string	_password;
		std::string	_apiKey;
		char		_bufRead[BUFFER_SIZE];
		Bot( void );

		static std::string	setPassword( std::string& password );
		static std::size_t	setPort( std::string& port );
		void				connect( int port, std::string& password );

	public:

		Bot( std::string port, std::string password, std::string apikey );
		~Bot( void );

		void						sendToServer( std::string str );
		int							readFromServer( void );
		int							execute( std::string &buffer );
		void						privmsg( std::string &msg );
		void						moderate( std::string &msg );

		std::vector< std::string >	splitBuffer( void );

};

/*---- structures -------------------------------------------------------------*/

typedef struct s_commands	t_commands;

struct s_commands
{
	std::string	key;
	void		(Bot::*function)( std::string &str );
};

/*---- prototypes -------------------------------------------------------------*/

std::string					splitCommand( std::string &buffer );
std::string					splitMessage( std::string &buffer );
std::vector< std::string >	splitArguments( std::string &s );

#endif