#include "irc.hpp"

void	mode(Server& s, Client& c, std::string& str)
{
	std::string target;
	size_t start = str.find(' ');
	size_t end = str.find(' ', start + 1);
	if (str[end - start - 1] == '\n')
		target = str.substr(start + 1, end - start - 1);
	else
		target = str.substr(start + 1, end - start - 1) + "\n";
	std::vector< Channel* > chan = s.getChannels();
	std::vector< Channel* >::iterator it;
	for (it = chan.begin(); it != chan.end(); ++it)
	{
		//for (size_t i = 0; i < (*it)->getName().size(); i++)
		//{
		//	std::cout << static_cast<int>(((*it)->getName())[i]) << " ";
		//}
		std::cout << "-" << (*it)->getName() << "-" << std::endl;
		std::cout << "-" << target << "-" << std::endl;
		if ((*it)->getName() == target) {
			std::cout << "channel inside s : " << (*it)->getName() << " and target : " << target << std::endl;
			break ;
		}
	}
	if (it == chan.end())
	{
		std::cout << "test" << std::endl;
		return ;
	}
}