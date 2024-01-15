/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:45:47 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 14:01:04 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

/*---- includes ---------------------------------------------------------------*/

# include "irc.h"
# include "Client.hpp"
# include "Channel.hpp"

/*---- class includes ---------------------------------------------------------*/

class Client;
class Channel;

/*---- class defines ----------------------------------------------------------*/

class	Server
{

	private:

		int								_listener;
		time_t							_creationTime;
		std::string						_name;
		std::string						_password;
		std::string						_motd;
		std::vector<Client*>			_newClients;
		std::map<std::string, Client*>	_clients;
		std::map<std::string, Channel*>	_channels;
		fd_set							_fdRead; // Ceux que le serveur doit lire

		Server();

		static int						setPort(std::string& portstr);
		static std::string				setPassword(std::string& pass);

		void							acceptClient();
		int								higherFd() const;
		void							removeClientFromChannels(Client& c);

	public:

		~Server();
		Server(std::string port, std::string password);

		std::map<std::string, Client*>	getClients() const;
		std::map<std::string, Channel*>	getChannels() const;
		std::string						getPass() const;
		std::string						getName() const;
		std::string						getMotd() const;
		time_t*							getCreationTime();

		int								getClientFd(std::string nickname);
		void							switchNick(std::string& prevNick, std::string& newNick);

		void							addClient(Client* client);
		void							removeClient(Client& c);
		void							addChannel(Channel* newChannel);
		void							removeChannel(Channel* chan);

		void							initFd();
		void							checkFd();

};

#endif