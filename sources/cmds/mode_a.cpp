#include "irc.hpp"
#include <stdlib.h>

void	opPrivilege(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		::sendToClient(c.getFd(), ERR_NOPRIVS(c.getNick()));
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
		::sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return ;
	}
	i++;
	std::string who;
	if (i < str.length()) {
		who = str.substr(i, str.length() - i);
	}
	else {
		::sendToClient(c.getFd(), ERR_NONICKNAMEGIVEN(c.getNick()));
		return ;
	}
	if (members.find(who) != members.end()) {
		::sendToClient(c.getFd(), ERR_USERNOTINCHANNEL(c.getNick(), who, ch.getName()));
		return ;
	}
	if (members[c.getNick()] == "~") {
		if (members[who] != "~") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				::sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
			else {
				ch.setPrivilege(who, "@");
				::sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
		}
	}
	else if (members[c.getNick()] == "@") {
		if (members[who] != "~" && members[who] != "@") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
				::sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
			else {
				ch.setPrivilege(who, "@");
				::sendToClient(c.getFd(), RPL_YOUREOPER(c.getNick()));
			}
		}
	}
}

void	setUserLimit(Client &c, Channel &ch, std::string str)
{
	std::map<std::string, std::string> members = ch.getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		::sendToClient(c.getFd(), ERR_NOPRIVS(c.getNick()));
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
	std::cout << str[i] << std::endl;
	size_t keep = i;
	for (i = keep; i < str.size(); i++) {
		if (isdigit(str[i]))
			break;
		else if (str[i] != ' ') {
			::sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", str[i], "not a digit or a space char..."));
			return ;
		}
	}
	keep = i;
	for (i = keep; i < str.size(); i++)
	{
		if (!isdigit(str[i])) {
			::sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), ch.getName(), "l", str[i], "there is a non digit char inside the parameter"));
			return ;
		}
		std::cout << str[i] << std::endl;
	}
	int	lim;
	std::string digitStr = str.substr(i, str.size() - 1);
	std::istringstream	ss(digitStr);

	ss >> lim;
	if (!ss.eof() || ss.fail()) {
		::sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), "", "l", digitStr, "is not a int."));
		return ;
	}
	if (ch.getNbUsers() <= lim) {
		ch.setUserLimit(lim);
	}
	else {
		::sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), "", "l", digitStr, "number of members is higher than the member limit"));
		return ;
	}
}

void	mode(Server& s, Client& c, std::string& str)
{
	std::string target;
	int start = str.find(' ');
	int end = str.find(' ', start + 1);
	if (end == -1)
		end = str.size() - 1;
	target = str.substr(start + 1, end - start - 1);
	std::map<std::string, Channel*>	chan = s.getChannels();
	if (!chan[target]) {
		::sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), target));
		return ;
	}
	std::string modeStr = str.substr(end, str.size() - (end));
	int modeOption = 0;
	for ( std::size_t i = 0; i <= modeStr.length(); i++ )
	{
		if ( modeStr[i] == '-' || modeStr[i] == '+' ) {
			modeOption = modeStr[i + 1];
			break;
		}
		else if ( modeStr[i] != ' ' ) {
			char errMode[2];
			errMode[0] = modeOption;
			errMode[1] = '\0';
			::sendToClient(c.getFd(), ERR_INVALIDMODEPARAM(c.getNick(), target, errMode, "", "not a minus (-) or plus (+) before mode option"));
			return ;
		}
	}
	switch (modeOption)
	{
	case i:
		/* code */
		break;
	case t:
		/* code */
		break;
	case k:
		/* code */
		break;
	case o:
		opPrivilege(c, *(chan[target]), modeStr);
		break;
	case l:
		setUserLimit(c, *(chan[target]), modeStr);
		break;
	default:
		::sendToClient(c.getFd(), ERR_UMODEUNKNOWNFLAG(c.getNick()));
		break;
	}
}