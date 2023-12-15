#include "irc.hpp"

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
		else if (str[i] != ' ') {
			std::cerr << "NO SIGN +/- ERROR" << std::endl;
			return ;
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
			}
			else {
				ch.setPrivilege(who, "@");
			}
		}
	}
	else if (members[c.getNick()] == "@") {
		if (members[who] != "~" && members[who] != "@") {
			if (sign == -1) {
				ch.setPrivilege(who, "");
			}
			else {
				ch.setPrivilege(who, "@");
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
		if (str[i] == '+') {
			break;
		}
	}
	i++;
	size_t keep = i + 1;
	int spaced = 0;
	int valid = 0;
	for (i = keep; i < str.size(); i++)
	{
		if (str[i] != ' ' && !isdigit(str[i])) {
			std::cerr << "WRONG CHAR ICI:" << str[i] << "." << std::endl;
			return ;
		}
		if (str[i] == ' ') {
			spaced = 1;
		}
		if (spaced == 1 && isdigit(str[i])) {
			valid = 1;
			break;
		}
	}
	if (valid == 0) {
		std::cerr << "STOIEF ERR" << std::endl;
		return ;
	}
	int	lim;
	std::string digitStr = str.substr(i, str.size() - 1);
	std::istringstream	ss(digitStr);

	ss >> lim;
	if (!ss.eof() || ss.fail()) {
		std::cerr << "ERROR NOT A INT" << std::endl;
		return ;
	}
	if (ch.getNbUsers() <= lim) {
		ch.setUserLimit(lim);
	}
	else
		std::cerr << "IMPOSSIBLE TOO MANY USER INSIDE ALREADY" << std::endl;
}

void	mode(Server& s, Client& c, std::string& str)
{
	std::string	target;

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
	std::string modeStr = str.substr(end, str.size() - end);
	int modeOption = 0;
	for ( std::size_t i = 0; i <= modeStr.length(); i++ )
	{
		if ( modeStr[i] == '-' || modeStr[i] == '+' ) {
			modeOption = modeStr[i + 1];
			break;
		}
	}
	switch (modeOption)
	{
	case i:
		i_opt(c, chan[target], modeStr);
		break;
	case t:
		/* code */
		break;
	case k:
		k_opt(c, chan[target], modeStr);
		break;
	case o:
		opPrivilege(c, *(chan[target]), modeStr);
		break;
	case l:
		setUserLimit(c, *(chan[target]), modeStr);
		break;
	default:
		char errMode[2];
		errMode[0] = modeOption;
		errMode[1] = '\0';
		::sendToClient(c.getFd(), ERR_UNKNOWNMODE(c.getNick(), errMode));
		break;
	}
}