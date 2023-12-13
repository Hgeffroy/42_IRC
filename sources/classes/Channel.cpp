/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/10 09:41:51 by hgeffroy         ###   ########.fr       */
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

std::map<std::string, std::string> Channel::getMembers() const
{
	return (_members);
}

std::string Channel::getTopic() const
{
	return (_topic);
}

bool	Channel::getUnderUserLimit() const
{
	if (_userLimit == -1)
		return (true);
	else if (_nbUsers < _userLimit)
		return (true);
	return (false);
}

int	Channel::getNbUsers() const
{
	return (_nbUsers);
}


void	Channel::setUserLimit( int lim )
{
	_userLimit = lim;
}


/**  Public member functions  *****************************************************************************************/

void	Channel::addUser(Client& newClient)
{
	_members[newClient.getNick()] = "";
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
}
