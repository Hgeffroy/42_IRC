/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:44:37 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 08:12:42 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

/*---- includes --------------------------------------------------------------*/

# include "irc.h"
# include "Server.hpp"

class Server;

class	Client
{

	private:

		int							_fd;
		bool						_connected;
		bool						_passwordOk;
		bool						_away;
		std::string 				_nickname; // Imposer de commencer par un alpha !
		std::string					_username; // Imposer de commencer par un alpha !
		char						_bufRead[BUFFER_SIZE]; // Ce que lit le serveur chez le client
		char						_bufWrite[BUFFER_SIZE]; // Ce que write le serveur au client
		std::string					_buffer;

		int							getCmd(std::string& buffer);
		int							execCmd(Server &s, std::string& str);
		std::vector<std::string>	splitBuf();

		int							setInfos(Server& s, std::string& str);

	public:

		Client(int socket);
		~Client();

		int				getFd() const;
		std::string		getNick() const;
		std::string		getUser() const;
		bool			getConnected() const;
		bool			getAway() const;

		void			setPassOk();
		void			setPassNotOk();
		void			setUser(std::string& str);
		void			setNick(Server& s, std::string& str);
		void			setAway(bool away);

		int 			read(Server& s);

};

#endif