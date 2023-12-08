/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/08 14:09:59 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Channel::Channel()
{

}

Channel::Channel(std::string& name, Client& op) : _name(name), _topic("Random topic")
{
	_members[op.getNick()] = "@";
	std::cout << "Channel " << _name << " was created" << std::endl;
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


/**  Public member functions  *****************************************************************************************/

void	Channel::addUser(Client& newClient)
{
	_members[newClient.getNick()] = "";
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
}
