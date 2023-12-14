#include "irc.hpp"

void	setUserLimit(Client &c, Channel *ch, std::string str)
{
	std::map<std::string, std::string> members = ch->getMembers();
	if (members[c.getNick()] != "@") {
		std::cerr << "NO OPERATOR PRIVILEGE" << std::endl;
		return ;
	}
	size_t	i = 0;
	for (i = 0; i < str.length(); i++)
	{
		if (str[i] == '-') {
			ch->setUserLimit(-1);
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
	if (ch->getNbUsers() <= lim) {
		ch->setUserLimit(lim);
		std::cout << "LIMIT IS ON=" << ch->getUserLimit() << std::endl;
	}
	else
		std::cerr << "IMPOSSIBLE TOO MANY USER INSIDE ALREADY" << std::endl;
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
		std::cerr << "NO CHANNEL" << std::endl;
		return ;
	}
	std::string modeStr = str.substr(end, str.size() - end);
	int modeOption = 0;
	std::cout << modeStr << std::endl;
	for ( std::size_t i = 0; i <= modeStr.length(); i++ )
	{
		if ( modeStr[i] == '-' || modeStr[i] == '+' ) {
			modeOption = modeStr[i + 1];
			break;
		}
	}
	std::cout << "Debug :" << static_cast<char>(modeOption) << "=" << std::endl;
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
		/* code */
		break;
	case l:
		setUserLimit(c, chan[target], modeStr);
		break;
	default:
		std::cerr << "Not an option that we coded" << std::endl;
		break;
	}
}