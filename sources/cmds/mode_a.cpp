#include "irc.hpp"

void	mode(Server& s, Client& c, std::string& str)
{
	std::string	target;

	int start = str.find(' ');
	int end = str.find(' ', start + 1);
	if (end == -1)
		end = str.size() - 1;
	target = str.substr(start + 1, end - start - 1);
	std::cout << "-" << target << "-" << std::endl;
	std::vector< Channel* > chan = s.getChannels();
	std::vector< Channel* >::iterator it;
	for (it = chan.begin(); it != chan.end(); ++it)
	{
		if ((*it)->getName() == target) {
			break ;
		}
	}
	if (it == chan.end()) {
		std::cerr << "DIDNT FIND A CHANNEL NAMED LIKE THIS" << std::endl;
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
		i_opt(chan, c, modeStr);
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
		/* code */
		break;
	default:
		std::cerr << "Not an option that we coded" << std::endl;
		break;
	}
}