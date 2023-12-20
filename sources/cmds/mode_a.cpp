#include "irc.hpp"

void	opPrivilege(Client &c, Channel &ch, std::string str)
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

void	setUserLimit(Client &c, Channel &ch, std::string str)
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

void	setTopicProtection(Client &c, Channel &ch, std::string str)
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
		case i:
			i_opt(c, chan[target], mode);
			break;
		case t:
			setTopicProtection(c, *(chan[target]), mode);
			break;
		case k:
			k_opt(c, chan[target], mode);
			break;
		case o:
			opPrivilege(c, *(chan[target]), mode);
			break;
		case l:
			setUserLimit(c, *(chan[target]), mode);
			break;
		case b:
			b_opt(c, chan[target], mode);
			break;
		default:
			sendToClient(c.getFd(), ERR_UNKNOWNMODE(c.getNick(), option));
			break;
	}
}