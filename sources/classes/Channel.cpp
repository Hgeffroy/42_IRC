/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:53:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:06:23 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/**  Constructors and destructors  *********************************************/

Channel::Channel(std::string const& name) : _name(name), _topic(""), \
											_userLimit(-1), _nbUsers(0), \
											_inviteOnly(false), _keyProtect(false), \
											_topicProtect(false)
{}

Channel::~Channel() {}

/**  Setters and Getters  ******************************************************/

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

void	Channel::setInviteStatus( bool status )
{
	_inviteOnly = status;
}

void	Channel::setKeyStatus( bool status )
{
	_keyProtect = status;
}

void	Channel::setPassword( std::string password )
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

/**  Public member functions  **************************************************/

void	Channel::refreshChanMembers(Server& s)
{
	std::map<std::string, Client*>					clientList = s.getClients();
	std::map<std::string, std::string>::iterator	it;

	for (it = _members.begin(); it != _members.end(); ++it) {
		Client* client = clientList[it->first];
		std::map<std::string, std::string>::iterator	it2;

		for (it2 = _members.begin(); it2 != _members.end(); ++it2) {
			std::string prefix = it2->second;
			if (it2->second == "~")
				prefix = "@";
			sendToClient(client->getFd(), RPL_NAMREPLY(client->getNick(), "=", _name, prefix + it2->first));
		}
		sendToClient(client->getFd(), RPL_ENDOFNAMES(_name));
	}
}

void	Channel::addUserToChan(Server& s, Client& newClient, bool isFounder)
{
	_members[newClient.getNick()] = "";
	if (isFounder)
		_members[newClient.getNick()] = "~";
	if ( newClient.getNick() != "bot" )
		_nbUsers++;

	refreshChanMembers(s);
}

int	Channel::removeUserFromChan(Server& s, std::string const& name)
{
	std::map<std::string, std::string>::iterator	it2 = _members.find(name);
	if (it2 != _members.end()) {
		if (it2->second == "~") {
			s.removeChannel(this);
			return (1);
		}
		_members.erase(it2);
		_nbUsers--;
	}
	return (0);
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

void	Channel::sendToChannel( Server& s, std::string str )
{
	std::map< std::string, std::string >			members = this->getMembers( );
	std::map< std::string, std::string >::iterator	it = members.begin( );
	std::map< std::string, Client* >				clients = s.getClients( );

	for ( it = members.begin( ); it != members.end( ); ++it ) {
		Client* client = clients.find( it->first )->second;
		sendToClient( client->getFd( ), str );
	}
}

void Channel::switchNick(std::string& prevNick, std::string& newNick)
{
	if (_members.find(prevNick) != _members.end()) {
		_members[newNick] = _members[prevNick];
		_members.erase(prevNick);
	}

	std::vector<std::string>::iterator it;
	for (it = _guestList.begin(); it != _guestList.end(); ++it) {
		if (*it == prevNick) {
			_guestList.push_back(newNick);
			_guestList.erase(it);
			break ;
		}
	}

	std::vector<std::string>::iterator it2;
	for (it2 = _bannedList.begin(); it2 != _bannedList.end(); ++it2) {
		if (*it2 == prevNick) {
			_bannedList.erase(it2);
			_bannedList.push_back(newNick);
			break ;
		}
	}
}
