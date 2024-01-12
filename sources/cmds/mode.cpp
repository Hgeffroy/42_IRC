/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:17:16 by twang             #+#    #+#             */
/*   Updated: 2024/01/12 11:02:58 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static std::string	getTarget( Client &c, std::string str );
static std::string	getMode( Client &c, Channel *channel, std::string str );

static int			opPrivilege(Client &c, Channel &ch, std::string str);
static int			setUserLimit(Client &c, Channel &ch, std::string str);
static int			setTopicProtection(Client &c, Channel &ch, std::string str);
static int			i_opt( Client &c, Channel *channel, std::string s );
static int			k_opt( Client &c, Channel *channel, std::string s );
static int			b_opt( Server& s, Client &c, Channel *channel, std::string str );

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
			if (!opPrivilege(c, *(chan[target]), mode))
				sendToClient(c.getFd(), MODE_MSG(c.getNick(), c.getUser(), str));
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
			b_opt(s, c, chan[target], mode);
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
static int	opPrivilege(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_NOPRIVS(c.getNick()));
		return (-1);
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
		return (-1);
	}
	i++;
	std::string who;
	if (i < str.length()) {
		who = str.substr(i, str.length() - i);
	}
	else {
		sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return (-1);
	}
	if (members.find(who) == members.end()) {
		sendToClient(c.getFd(), ERR_USERNOTINCHANNEL(c.getNick(), who, ch.getName()));
		return (-1);
	}
	if (members[c.getNick()] == "~") {
		if (members[who] != "~") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				std::cout << PURPLE << "MODE " << ch.getName() << " -o";
				std::cout << " : Removed Op privilege to " << who << " on " << ch.getName() << END << std::endl;
			}
			else {
				ch.setPrivilege(who, "@");
				std::cout << YELLOW << "MODE " << ch.getName() << " +o";
				std::cout << " : Gave Op privilege to " << who << " on " << ch.getName() << END << std::endl;
			}
		}
	}
	else if (members[c.getNick()] == "@") {
		if (members[who] != "~" && members[who] != "@") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				std::cout << PURPLE << "MODE " << ch.getName() << " -o";
				std::cout << " : Removed Op privilege to " << who << " on " << ch.getName() << END << std::endl;
			}
			else {
				ch.setPrivilege(who, "@");
				std::cout << YELLOW << "MODE " << ch.getName() << " +o";
				std::cout << " : Gave Op privilege to " << who << " on " << ch.getName() << END << std::endl;
			}
		}
	}
	return (0);
}

//l: Set/remove User Limit on a channel
static int	setUserLimit(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), ch.getName()));
		return (-1);
	}
	size_t	i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			if (str[i + 2] == '\0') {
				ch.setUserLimit(-1);
				std::cout << PURPLE << "MODE " << ch.getName() << " -l";
				std::cout << " : Unset the \"user limit\" mode on " << ch.getName() << END << std::endl;
			}
			return (-1);
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
			return (-1);
		}
	}
	keep = i;
	for (i = keep; i < str.size(); i++)
	{
		if (!isdigit(str[i])) {
			sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", str[i], "there is a non digit char inside the parameter"));
			return (-1);
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
		return (-1);
	}
	if (ch.getNbUsers() <= lim) {
		ch.setUserLimit(lim);
		std::cout << YELLOW << "MODE " << ch.getName() << " +l";
		std::cout << " : Set the \"user limit\" mode on " << ch.getName() << " to " << lim << END << std::endl;
	}
	else {
		sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", digitStr, "number of members is higher than the member limit"));
		return (-1);
	}
	return (0);
}

//t: Set/remove Topic Protection on a channel
static int	setTopicProtection(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), ch.getName()));
		return (-1);
	}
	size_t	i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			if (str[i + 2] == '\0') {
				ch.setTopicProtect(false);
				std::cout << PURPLE << "MODE " << ch.getName() << " -t";
				std::cout << " : Unset the \"topic protection\" mode on " << ch.getName() << END << std::endl;
			}
			else {
				sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "t", "", "MUST NOT input an argument to this parameter"));
			}
			return (-1);
		}
		else if (str[i] == '+') {
			if (str[i + 2] == '\0') {
				ch.setTopicProtect(true);
				std::cout << YELLOW << "MODE " << ch.getName() << " +t";
				std::cout << " : Set the \"topic protection\" mode on " << ch.getName() << END << std::endl;
			}
			else {
				sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "t", "", "MUST NOT input an argument to this parameter"));
			}
			return (-1);
		}
	}
	return (0);
}

//i: Set/remove Invite-only channel
static int	i_opt( Client &c, Channel *channel, std::string s )
{
	std::string	option = getOption( c, 'i', s );
	if ( option.empty() )
		return (-1);

	if ( !isOperator( c, channel ) )
		return (-1);

	if ( s[0] == '+' )
	{
		if ( channel->getInviteStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return (-1);
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
			return (-1);
		}
		channel->setInviteStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -i";
		std::cout << " : Unset the \"invite only\" mode on " << channel->getName() << END << std::endl;
	}
	return (0);
}

//k: Set/remove the channel key (password)
static int	k_opt( Client &c, Channel *channel, std::string s )
{
	if ( !isOperator( c, channel ) )
		return (-1);

	std::string	option = getOption( c, 'k', s );
	if ( option.empty( ) )
		return (-1);

	if ( s[0] == '+' )
	{
		std::string	password = getParam( c, s );
		if ( password.empty( ) || password.size( ) < 6 )
			return (-1);
		if ( channel->getKeyStatus( ) )
		{
			sendToClient( c.getFd(), RPL_CHANNELMODEIS( c.getNick(), channel->getName(), channel->getModes(), "*parameters" ) );
			return (-1);
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
			return (-1);
		}
		channel->setKeyStatus( false );
		std::cout << PURPLE << "MODE " << channel->getName() << " -k";
		std::cout << " : Unset the \"key protect\" mode on " << channel->getName() << END << std::endl;
	}
	return (0);
}


//b: Ban/un-ban client from channel
static int	b_opt( Server& s, Client &c, Channel *channel, std::string str )
{
	std::map< std::string, std::string >	members = channel->getMembers();
	std::vector< std::string >				bannedList = channel->getBannedGuest();
	std::string								option = getOption( c, 'b', str );
	if ( option.empty() )
		return (-1);

	if ( !isOperator( c, channel ) )
		return (-1);

	if ( str[0] == '+' )
	{
		std::string	banned = getParam( c, str );
		if ( banned.empty( ) )
			return (-1);
		std::map<std::string, std::string>::iterator ite = members.find( banned );
		if ( ite != members.end() )
		{
			if ( members[ banned ] == "~" )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban a founder from channel."));
				return (-1);
			}
			if ( members[ banned ] == "@" )
			{
				if ( members[ c.getNick() ] != "~" )
				{
					sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " cannot ban an operator from channel."));
					return (-1);
				}
			}
			channel->setBannedGuest( banned );
			channel->removeUserFromChan( s, banned );
			channel->sendToChannel( s, PART_MSG( banned, banned, channel->getName( ), "This creep has been banned" ) );
			channel->removeUserFromGuestList( banned );
			std::cout << YELLOW << "MODE " << channel->getName() << " +b";
			std::cout << " : Setting the \"ban\" mode for : <" << banned << ">" << END << std::endl;
			return (-1);
		}
		for ( std::vector< std::string >::iterator	itb = bannedList.begin(); itb != bannedList.end(); itb++ )
		{
			if ( *itb == banned )
			{
				sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "+b", banned, " already banned from this channel."));
				return (-1);
			}
		}
		sendToClient( c.getFd(), ERR_NOTONCHANNEL( banned, channel->getName() ) );
		return (-1);
	}
	else if ( str[0] == '-' )
	{
		std::string	unbanned = getParam( c, str );
		if ( unbanned.empty( ) )
			return (-1);
		if ( isBanned( channel, unbanned ) )
		{
			channel->removeUserFromBanList( unbanned );
			std::cout << PURPLE << "MODE " << channel->getName() << " -b";
			std::cout << " : Unsetting the \"ban\" mode for : <" << unbanned << ">" << END << std::endl;
			return (-1);
		}
		sendToClient( c.getFd(), ERR_INVALIDMODEPARAM( c.getNick(), channel->getName(), "-b", unbanned, " was not banned from this channel."));
		return (-1);
	}
	return (0);
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
