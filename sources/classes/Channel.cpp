/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 16:17:11 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Channel::Channel()
{

}

Channel::Channel(std::string const& name, std::string const& founder) : _name(name), _topic(""), _userLimit(-1), _nbUsers(1)
{
	_members[founder] = "@";
	std::cout << "Channel -" << _name << "- was created" << std::endl;
}

Channel::~Channel()
{

}

/**  Setters and Getters  *********************************************************************************************/

std::string Channel::getName() const
{
	return(_name);
}

std::string Channel::getTopic() const
{
	return (_topic);
}

std::map<std::string, std::string> Channel::getMembers() const
{
	return (_members);
}

bool	Channel::getInviteStatus() const
{
	return (_inviteOnly);
}

bool	Channel::getKeyStatus() const
{
	return (_keyProtect);
}

std::string	Channel::getPassword() const
{
	return (_password);
}

void	Channel::setInviteStatus(bool status)
{
	_inviteOnly = status;
}

void	Channel::setKeyStatus(bool status)
{
	_keyProtect = status;
}

void	Channel::setPassword(std::string password)
{
	_password = password;
}

bool	Channel::underUserLimit() const
{
	if (_userLimit == -1)
		return (true);
	else if (_nbUsers < _userLimit)
		return (true);
	return (false);
}


/**  Public member functions  *****************************************************************************************/

void	Channel::addUser(Client& newClient)
{
	_members[newClient.getNick()] = "";
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
}
