/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 09:30:24 by twang             #+#    #+#             */
/*   Updated: 2024/01/05 10:17:37 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_CPP
# define BOT_CPP

# include "bot.h"

class Bot
{

	private:

		std::string	_addr;
		int			_port;
		int			_socket;
		std::string	_passwd;
		std::string	_nick;
		std::string	_user;
		Bot( void );

	public:

		Bot( std::string port, std::string password, std::string apikey );
		~Bot( void );

};

#endif