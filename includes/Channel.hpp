/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:39:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/14 10:10:21 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.h"

class	Client;

class	Channel
{

private:

	const std::string					_name;
	std::map<std::string, std::string>	_members;
	std::string 						_topic;
	int									_userLimit;
	int									_nbUsers;

	bool								_inviteOnly;
	bool								_keyProtect;
	std::string							_password;

	Channel();

public:

	Channel(std::string const& name, std::string const& founder);
	~Channel();

	std::string 						getName() const;
	std::string 						getTopic() const;
	std::map<std::string, std::string>	getMembers() const;
	int									getNbUsers() const;
	int									getUserLimit() const;
	void								setUserLimit( int lim );

	bool								getInviteStatus() const;
	bool								getKeyStatus() const;
	std::string							getPassword() const;

	void								setInviteStatus(bool status);
	void								setKeyStatus(bool status);
	void								setPassword(std::string password);

	void								addUser(Client& newClient);

};

#endif