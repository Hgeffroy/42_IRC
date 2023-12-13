#include "irc.hpp"

void	mode(Server& s, Client& c, std::string& str)
{
	std::string target;
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
}