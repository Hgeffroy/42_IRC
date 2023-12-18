#include "irc.hpp"

void	topic(Server& s, Client& c, std::string& str)
{
	std::string	target;

	int start = str.find(' ');
	int end = str.find(' ', start + 1);
	if (end == -1)
		end = str.size();
	target = str.substr(start + 1, end - start - 1);
	std::map<std::string, Channel*>	chan = s.getChannels();
	if (!chan[target]) {
		sendToClient(c.getFd(), ERR_NOSUCHCHANNEL(c.getNick(), target));
		return ;
	}
	if (str.size() - end == 0) {
		sendToClient(c.getFd(), RPL_TOPIC(c.getNick(), target, chan[target]->getTopic()));
		return;
	}
	size_t i;
	for (i = end; i < str.size(); i++)
	{
		if (str[i] == ' ' || str[i] == ':')
			continue;
		else
			break;
	}
	end = i;
	std::string modeStr = str.substr(end, str.size() - (end));
	std::cout << "=" << modeStr << "=" << std::endl;
	if (chan[target]->getTopicProtect() == true)
	chan[target]->setTopic(modeStr);
}