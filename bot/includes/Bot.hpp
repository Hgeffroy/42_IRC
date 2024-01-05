/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:30:24 by twang             #+#    #+#             */
/*   Updated: 2024/01/05 15:59:48 by twang            ###   ########.fr       */
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

		// std::string	_addr; -> voir avec arrhur?
		std::size_t	_socket;
		std::string	_password;
		Bot( void );

		static std::string	setPassword( std::string& password );
		static std::size_t	setPort( std::string& port );
		void				connect( int port, std::string& password );

	public:

		Bot( std::string port, std::string password, std::string apikey );
		~Bot( void );

		void	sendToServer( std::string str );

};

#endif