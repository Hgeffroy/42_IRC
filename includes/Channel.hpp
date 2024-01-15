/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:39:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:06:35 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

/*---- includes --------------------------------------------------------------*/

# include "irc.h"

/*-----class defines ---------------------------------------------------------*/

class	Client;
class	Server;

/*----------------------------------------------------------------------------*/

class	Channel
{

	private:

		std::map<std::string, std::string>	_members;
		std::vector< std::string >			_guestList;
		std::vector< std::string >			_bannedList;
		const std::string					_name;
		std::string 						_topic;
		std::string							_password;
		int									_userLimit;
		int									_nbUsers;
		bool								_inviteOnly;
		bool								_keyProtect;
		bool								_topicProtect;


	public:

		Channel(std::string const& name);
		~Channel();

		std::string 						getName() const;
		std::string 						getTopic() const;
		std::map<std::string, std::string>	getMembers() const;
		int									getNbUsers() const;
		int									getUserLimit() const;
		bool								getTopicProtect() const;
		bool								getInviteStatus() const;
		bool								getKeyStatus() const;
		std::string							getPassword() const;
		std::vector< std::string >			getGuest() const;
		std::vector< std::string >			getBannedGuest() const;
		std::string							getModes() const;

		void								setUserLimit( int lim );
		void								setTopicProtect( bool b );
		void								setPrivilege( std::string who, std::string privilege );
		void								setInviteStatus( bool status );
		void								setKeyStatus ( bool status );
		void								setPassword( std::string password );
		void								setGuest( std::string guest );
		void								setBannedGuest( std::string banned );
		void								setTopic( std::string topic );

		int									removeUserFromChan(Server& s, std::string const& name);
		void								addUserToChan(Server& s, Client& newClient, bool isFounder);
		void								removeUserFromGuestList(std::string const& name);
		void								removeUserFromBanList(std::string const& name);
		void								sendToChannel( Server& s, std::string str );
		void								refreshChanMembers(Server& s);
		void								switchNick( std::string& prevNick, std::string& newNick );


};

#endif