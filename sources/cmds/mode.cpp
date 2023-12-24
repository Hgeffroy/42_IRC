/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wangthea <wangthea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:17:16 by twang             #+#    #+#             */
/*   Updated: 2023/12/24 15:46:25 by wangthea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::string	getTarget( Client &c, std::string str );
static std::string	getMode( Client &c, Channel *channel, std::string str );

static void			opPrivilege(Client &c, Channel &ch, std::string str);
static void			setUserLimit(Client &c, Channel &ch, std::string str);
static void			setTopicProtection(Client &c, Channel &ch, std::string str);
static void			i_opt( Client &c, Channel *channel, std::string s );
static void			k_opt( Client &c, Channel *channel, std::string s );
static void			b_opt( Client &c, Channel *channel, std::string s );

static bool			isOperator( Client &c, Channel *channel );
static bool			isBanned( Channel *channel, std::string unbanned );
static std::string	getOption( Client &c, char opt, std::string s );
static std::string	getParam( Client &c, std::string s );

/*----------------------------------------------------------------------------*/

void	mode(Server& s, Client& c, std::string& str)
{
	std::string	target = getTarget( c, str );
	if ( target.empty() )
		return ;

	std::map<std::string, Channel*>	chan = s.getChannels();
	if (!chan[target])
	{
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), target));
		return ;
	}

	std::string	mode = getMode( c, chan[target], str );
	if ( mode.empty() )
		return ;

	char option = mode[1];
	switch ( option )
	{
		case o:
			opPrivilege(c, *(chan[target]), mode);
			break;
		case l:
			setUserLimit(c, *(chan[target]), mode);
			break;
		case t:
			setTopicProtection(c, *(chan[target]), mode);
			break;
		case i:
			i_opt(c, chan[target], mode);
			break;
		case k:
			k_opt(c, chan[target], mode);
			break;
		case b:
			b_opt(c, chan[target], mode);
			break;
		default:
			sendToClient(c.getFd(), ERR_UNKNOWNMODE(c.getNick(), option));
			break;
	}
}

/*---- utils ------------------------------------------------------------------*/

static std::string	getTarget( Client &c, std::string str )
{
	std::string	target;

	std::size_t	first_space = str.find( ' ' );
	std::size_t	second_space = str.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		target = str.substr( first_space + 1, second_space - first_space - 1 );
		if ( target.empty() )
		{
			sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE <channel> <+ | -><mode> [*parameters]" ) );
			return ( "" );
		}
		return ( target );
	}
	target = str.substr( first_space + 1 );
	if ( target.empty() )
	{
		sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE <channel> <+ | -><mode> [*parameters]" ) );
		return ( "" );
	}
	return ( target );
}

static std::string	getMode( Client &c, Channel *channel, std::string str )
{
	std::string	mode;

	std::size_t	first_space = str.find( ' ' );
	std::size_t	second_space = str.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		mode = str.substr( second_space + 1 );
		if ( mode.empty() )
		{
			sendToClient(c.getFd(), RPL_CHANNELMODEIS(c.getNick(), channel->getName(), channel->getModes(), "*parameters"));
			return ( "" );
		}
		if ( mode[0] != '+' && mode[0] != '-' )
		{
			sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
			sendToClient(c.getFd(), RPL_CHANNELMODEIS(c.getNick(), channel->getName(), channel->getModes(), "*parameters"));
			return ( "" );
		}
		return ( mode );
	}
	sendToClient(c.getFd(), RPL_CHANNELMODEIS(c.getNick(), channel->getName(), channel->getModes(), "*parameters"));
	return ( "" );
}

/*---- options ---------------------------------------------------------------*/

//o: Set/remove Operator privilege on a client
static void	opPrivilege(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_NOPRIVS(c.getNick()));
		return ;
	}
	size_t	i = 0;
	int sign = 1;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			sign = -1;
			break;
		}
		else if (str[i] == '+') {
			break;
		}
	}
	i += 2;
	if (str[i] != ' ') {
		sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return ;
	}
	i++;
	std::string who;
	if (i < str.length()) {
		who = str.substr(i, str.length() - i);
	}
	else {
		sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return ;
	}
	if (members.find(who) == members.end()) {
		sendToClient(c.getFd(), ERR_USERNOTINCHANNEL(c.getNick(), who, ch.getName()));
		return ;
	}
	if (members[c.getNick()] == "~") {
		if (members[who] != "~") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				//sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
			else {
				ch.setPrivilege(who, "@");
				//sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
		}
	}
	else if (members[c.getNick()] == "@") {
		if (members[who] != "~" && members[who] != "@") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				//sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
			else {
				ch.setPrivilege(who, "@");
				//sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
		}
	}
}

//l: Set/remove User Limit on a channel
static void	setUserLimit(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), ch.getName()));
		return ;
	}
	size_t	i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			if (str[i + 2] == '\0') {
				ch.setUserLimit(-1);
			}
			return;
		}
		else if (str[i] == '+') {
			break;
		}
	}
	i += 2;
	size_t keep = i;
	for (i = keep; i < str.size(); i++) {
		if (str[i] == ' ')
			continue;
		else if (isdigit(str[i]) && i != keep)
			break;
		else {
			sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", str[i], "not a digit or a space char..."));
			return ;
		}
	}
	keep = i;
	for (i = keep; i < str.size(); i++)
	{
		if (!isdigit(str[i])) {
			sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", str[i], "there is a non digit char inside the parameter"));
			return ;
		}
	}
	i--;
	std::cout << str[i] << std::endl;
	int	lim;
	int	len = str.size() - i;
	std::string digitStr = str.substr(i, len);
	std::istringstream	ss(digitStr);

	ss >> lim;
	if (!ss.eof() || ss.fail()) {
		sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", digitStr, "is not a int."));
		return ;
	}
	if (ch.getNbUsers() <= lim) {
		ch.setUserLimit(lim);
	}
	else {
		sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", digitStr, "number of members is higher than the member limit"));
		return ;
	}
}

//t: Set/remove Topic Protection on a channel
static void	setTopicProtection(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), ch.getName()));
		return ;
	}
	size_t	i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			if (str[i + 2] == '\0') {
				ch.setTopicProtect(false);
			}
			else {
				sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "t", "", "MUST NOT input an argument to this parameter"));
			}
			return;
		}
		else if (str[i] == '+') {
			if (str[i + 2] == '\0') {
				ch.setTopicProtect(true);
			}
			else {
				sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "t", "", "MUST NOT input an argument to this parameter"));
			}
			return;
		}
	}
}

//i: Set/remove Invite-only channel
static void	i_opt( Client &c, Channel *channel, std::string s )
{
	std::string	option = getOption( c, 'i', s );
	if ( option.empty() )
		return ;

	if ( !isOperator( c, channel ) )
		return ;

	if ( s[0] == '+' )
	{
		if ( channel->getInviteStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setInviteStatus( true );
		std::cout << YELLOW << "MODE " << channel->getName() << " +i";
		std::cout << " : Setting the \"invite only\" mode on " << channel->getName() << END << std::endl;
	}
	else if ( s[0] == '-' )
	{
		if ( !channel->getInviteStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setInviteStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -i";
		std::cout << " : Unset the \"invite only\" mode on " << channel->getName() << END << std::endl;
	}
}

//k: Set/remove the channel key (password)
static void	k_opt( Client &c, Channel *channel, std::string s )
{
	if ( !isOperator( c, channel ) )
		return ;

	std::string	option = getOption( c, 'k', s );
	if ( option.empty( ) )
		return ;

	if ( s[0] == '+' )
	{
		std::string	password = getParam( c, s );
		if ( password.empty( ) || password.size( ) < 6 )
			return ;
		if ( channel->getKeyStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setKeyStatus( true );
		channel->setPassword( password );
		std::cout << YELLOW << "MODE " << channel->getName() << " +k";
		std::cout << " : Setting the \"key protect\" mode on " << channel->getName();
		std::cout << " with key : <" << channel->getPassword() << ">" << END << std::endl;
	}
	else if ( s[0] == '-' )
	{
		if ( !channel->getKeyStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return ;
		}
		channel->setKeyStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -k";
		std::cout << " : Unset the \"key protect\" mode on " << channel->getName() << END << std::endl;
	}
}


//b: Ban/un-ban client from channel
static void	b_opt( Client &c, Channel *channel, std::string s )
{
	std::map< std::string, std::string >	members = channel->getMembers();
	std::vector< std::string >				bannedList = channel->getBannedGuest();
	std::string								option = getOption( c, 'b', s );
	if ( option.empty() )
		return ;

	if ( !isOperator( c, channel ) )
		return ;

	if ( s[0] == '+' )
	{
		std::string	banned = getParam( c, s );
		if ( banned.empty( ) )
			return ;
		std::map<std::string, std::string>::iterator ite = members.find( banned );
		if ( ite != members.end() )
		{
			if ( members[ banned ] == "~" )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban a founder from channel."));
				return ;
			}
			if ( members[ banned ] == "@" )
			{
				if ( members[ c.getNick() ] != "~" )
				{
					sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban an operator from channel."));
					return ;
				}
			}
			channel->setBannedGuest( banned );
			channel->removeUserFromChan( banned );
			channel->removeUserFromGuestList( banned );
			std::cout << YELLOW << "MODE " << channel->getName() << " +b";
			std::cout << " : Setting the \"ban\" mode for : <" << banned << ">" << END << std::endl;
			return ;
		}
		for ( std::vector< std::string >::iterator	itb = bannedList.begin(); itb != bannedList.end(); itb++ )
		{
			if ( *itb == banned )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " already banned from this channel."));
				return ;
			}
		}
		sendToClient( c.getFd(), ERR_NOTONCHANNEL( banned, channel->getName() ) );
		return ;
	}
	else if ( s[0] == '-' )
	{
		std::string	unbanned = getParam( c, s );
		if ( unbanned.empty( ) )
			return ;
		if ( isBanned( channel, unbanned ) )
		{
			channel->removeUserFromBanList( unbanned );
			std::cout << PURPLE << "MODE " << channel->getName() << " -b";
			std::cout << " : Unsetting the \"ban\" mode for : <" << unbanned << ">" << END << std::endl;
			return ;
		}
		sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "-b", unbanned, " was not banned from this channel."));
	}
}

/*---- utils ------------------------------------------------------------------*/

static bool	isOperator( Client &c, Channel *channel )
{
	std::map< std::string, std::string >members = channel->getMembers();
	if ( members[ c.getNick( ) ] != "@" && members[ c.getNick( ) ] != "~" )
	{
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED( c.getNick(), channel->getName() ) );
		return ( false );
	}
	return ( true );
}

static bool	isBanned( Channel *channel, std::string unbanned )
{
	std::vector< std::string >	bannedList = channel->getBannedGuest();

	for ( std::vector< std::string >::iterator	it = bannedList.begin(); it != bannedList.end(); it++ )
	{
		if ( *it == unbanned )
			return ( true );
	}
	return ( false );
}

static std::string	getOption( Client &c, char opt, std::string s )
{
	if ( opt == 'i' )
	{
		std::string	option = s;
		if ( option.size() != 2 )
		{
			sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
			return ( "" );
		}
		return ( option );
	}
	if ( s[0] == '+' )
	{
		std::size_t	first_space = s.find( ' ' );
		if ( first_space != std::string::npos )
		{
			std::string	option = s.substr( 0, first_space );
			if ( option.size() != 2 )
			{
				sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
				return ( "" );
			}
			return ( option );
		}
	}
	if ( opt == 'b' )
	{
		if ( s[0] == '-' )
		{
			std::size_t	first_space = s.find( ' ' );
			if ( first_space != std::string::npos )
			{
				std::string	option = s.substr( 0, first_space );
				if ( option.size() != 2 )
				{
					sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
					return ( "" );
				}
				return ( option );
			}
		}
	}
	else if ( s == "-k" )
		return ( s );
	sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "MODE #lol +k <password>" ) );
	return ( "" );
}

static std::string	getParam( Client &c, std::string s )
{
	std::string	parameter;

	std::size_t	first_space = s.find( ' ' );
	std::size_t	second_space = s.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG( c.getNick() ));
		return ( "" );
	}
	if ( first_space != std::string::npos )
	{
		parameter = s.substr( first_space + 1 );
		if ( parameter.empty() )
		{
			sendToClient( c.getFd(), ERR_NEEDMOREPARAMS( c.getNick(), "ex: MODE #lol +k <parameter>" ) );
			return ( "" );
		}
	}
	return ( parameter );
}
