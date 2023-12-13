#include "irc.hpp"

void	setUserLimit(Server &s, Client &c, Channel *ch, std::string str)
{
	std::map<std::string, std::string> members = ch->getMembers();
	std::cout << members[c.getNick()] << std::endl;
	if (members[c.getNick()] != "@") {
		std::cerr << "NO OPERATOR PRIVILEGE" << std::endl;
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
	std::cout << "-" << target << "-" << std::endl;
	std::map<std::string, Channel*>	chan = s.getChannels();
	if (!chan[target]) {
		std::cerr << "NO CHANNEL" << std::endl;
		return ;
	}
	std::string modeStr = str.substr(end, str.size() - end - 1);
	std::cout << "=" << modeStr << "=" << std::endl;
	int modeOption = 0;
	for ( std::size_t i = 0; i < modeStr.length(); i++ )
	{
		if ( modeStr[i] == '-' || modeStr[i] == '+' )
			modeOption = modeStr[i + 1];
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
		/* code */
		break;
	case l:
		setUserLimit(s, c, chan[target], modeStr);
		break;
	default:
		std::cerr << "Not an option that we coded" << std::endl;
		break;
	}
}