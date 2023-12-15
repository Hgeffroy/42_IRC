/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:39:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/15 12:59:20 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

/*---- includes --------------------------------------------------------------*/

# include "irc.h"

/*-----class defines ---------------------------------------------------------*/

class	Client;

/*----------------------------------------------------------------------------*/

class	Channel
{

	private:

		std::map<std::string, std::string>	_members;
		std::vector< std::string >			_guestList;
		const std::string					_name;
		std::string 						_topic;
		std::string							_password;
		int									_userLimit;
		int									_nbUsers;
		bool								_inviteOnly;
		bool								_keyProtect;

		Channel();

	public:

		Channel(std::string const& name, std::string const& founder);
		~Channel();

		std::string 						getName() const;
		std::string 						getTopic() const;
		std::map<std::string, std::string>	getMembers() const;
		int									getNbUsers() const;
		int									getUserLimit() const;
		bool								getInviteStatus() const;
		bool								getKeyStatus() const;
		std::string							getPassword() const;

		void								setUserLimit( int lim );
		void								setPrivilege( std::string who, std::string privilege );
		void								setInviteStatus( bool status );
		void								setKeyStatus ( bool status );
		void								setPassword( std::string password );

		void								addUserToChan(Client& newClient);
		void								removeUserFromChan(Client& client);

};

#endif