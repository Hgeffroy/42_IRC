#include "irc.hpp"

void	printStrVec(std::vector<std::string> v)
{
	std::vector<std::string>::iterator it;
	int i = 0;

	for (it = v.begin(); it != v.end(); ++it) {
		std::cout << "v[" << i << "] = " << *it << std::endl;
		i++;
	}
}