#include "irc.hpp"

void	mode(Server& s, Client& c, std::string& str)
{
	size_t end = str.find(' ');
	std::cout << "mode: Espace is at =" << end << std::endl;
}