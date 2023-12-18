/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/15 09:42:13 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Channel::Channel()
{

}

Channel::Channel(std::string const& name, std::string const& founder) : _name(name), _topic(""), \
																		_userLimit(-1), _nbUsers(1), \
																		_inviteOnly(false), _keyProtect(false)
{
	_members[founder] = "~";
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

std::vector< std::string >	Channel::getGuest() const
{
	return ( _guestList );
}

int	Channel::getUserLimit() const
{
	return (_userLimit);
}

int	Channel::getNbUsers() const
{
	return (_nbUsers);
}

void	Channel::setUserLimit( int lim )
{
	_userLimit = lim;
}

void	Channel::setPrivilege( std::string who, std::string privilege )
{
	_members[who] = privilege;
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

void	Channel::setGuest( std::string guest )
{
	_guestList.push_back( guest );
}

/**  Public member functions  *****************************************************************************************/

void	Channel::addUserToChan(Client& newClient)
{
	_members[newClient.getNick()] = "";
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
	_nbUsers++;
}

void	Channel::removeUserFromChan(Client& client)
{
	std::map<std::string, std::string>::iterator	it = _members.find(client.getNick());

	if (it != _members.end())
	{
		_members.erase(it->first);
		_nbUsers--;
	}
}
