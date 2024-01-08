/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/07 14:42:04 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  ************************************************************************************/

Channel::Channel()
{

}

Channel::Channel(std::string const& name) : _name(name), _topic(""), \
																		_userLimit(-1), _nbUsers(0), \
																		_inviteOnly(false), _keyProtect(false), \
																		_topicProtect(false)
{
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

std::vector< std::string >	Channel::getBannedGuest() const
{
	return ( _bannedList );
}

int	Channel::getUserLimit() const
{
	return (_userLimit);
}

int	Channel::getNbUsers() const
{
	return (_nbUsers);
}

bool	Channel::getTopicProtect() const
{
	return (_topicProtect);
}

std::string	Channel::getModes() const
{
	std::string	modes;

	if( _inviteOnly )
		modes += "+i ";
	else
		modes += "-i ";

	if( _keyProtect )
		modes += "+k ";
	else
		modes += "-k ";

	if( _topicProtect )
		modes += "+t ";
	else
		modes += "-t ";

	if ( _userLimit > 0 )
		modes += "+l ";
	else
		modes += "-l ";
	if ( _bannedList.empty() )
		modes += "-b";
	else
		modes += "+b";

	return ( modes );
}

void	Channel::setUserLimit( int lim )
{
	_userLimit = lim;
}

void	Channel::setTopicProtect( bool b )
{
	_topicProtect = b;
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

void	Channel::setBannedGuest( std::string banned )
{
	_bannedList.push_back( banned );
}


void	Channel::setTopic( std::string topic )
{
	_topic = topic;
}


/**  Private member functions  ****************************************************************************************/

void	Channel::refreshChanMembers(Server& s)
{
	std::map<std::string, Client*>		clientList = s.getClients();
	std::map<std::string, std::string>::iterator	it;
	for (it = _members.begin(); it != _members.end(); ++it)
	{
		Client* client = clientList[it->first];
		std::map<std::string, std::string>::iterator	it2;
		for (it2 = _members.begin(); it2 != _members.end(); ++it2)
		{
			std::string prefix = it2->second;
			if (it2->second == "~")
				prefix = "@";
			sendToClient(client->getFd(), RPL_NAMREPLY(client->getNick(), "=", _name, prefix + it2->first));
		}
		sendToClient(client->getFd(), RPL_ENDOFNAMES(_name));
	}
}

/**  Public member functions  *****************************************************************************************/

void	Channel::addUserToChan(Server& s, Client& newClient, bool isFounder)
{
	_members[newClient.getNick()] = "";
	if (isFounder)
		_members[newClient.getNick()] = "~";
	std::cout << newClient.getNick() << " joined channel " << _name << std::endl;
	_nbUsers++;

	refreshChanMembers(s);
}

void	Channel::removeUserFromChan(Server& s, std::string const& name)
{
	std::map<std::string, std::string>::iterator	it2 = _members.find(name);

	if (it2 != _members.end())
	{
		_members.erase(it2);
		_nbUsers--;
	}

	refreshChanMembers(s);
}

void	Channel::removeUserFromGuestList(std::string const& name)
{
	for ( std::vector< std::string >::iterator	it = _guestList.begin(); it != _guestList.end(); it++ )
	{
		if ( *it == name )
		{
			it = _guestList.erase( it );
			return ;
		}
	}
}

void	Channel::removeUserFromBanList(std::string const& name)
{
	for ( std::vector< std::string >::iterator	it = _bannedList.begin(); it != _bannedList.end() ; it++ )
	{
		if ( *it == name )
		{
			it = _bannedList.erase( it );
			return ;
		}
	}
}
