/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/06 08:23:28 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Channel::Channel()
{

}

Channel::Channel(std::string& name, Client& op) : _name(name)
{
	_members.push_back(op);
	_ops.push_back(op);
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

std::vector<Client> Channel::getMembers() const
{
	return (_members);
}

/**  Public member functions  *****************************************************************************************/

void	Channel::addUser(Client& newClient)
{
	_members.push_back(newClient);
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
}
