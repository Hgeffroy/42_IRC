#include "irc.hpp"

void	kick(Server& s, Client& c, std::string& str)
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
	std::map<std::string, std::string> members = (*chan[target]).getMembers();
	if (members[c.getNick()] != "@" && members[c.getNick()] != "~") {
		sendToClient(c.getFd(), ERR_CHANOPRIVSNEEDED(c.getNick(), (*chan[target]).getName()));
		return ;
	}
	if (members.find(c.getNick()) == members.end()) {
		sendToClient(c.getFd(), ERR_NOTONCHANNEL(c.getNick(), (*chan[target]).getName()));
		return;
	}
	if (str.size() - end == 0) {
		sendToClient(c.getFd(), ERR_NEEDMOREPARAMS(c.getNick(), "KICK"));
	}

	std::string	user;
	int startTwo = str.find(' ', end);
	end = str.find(' ', startTwo + 1);
	if (end == -1)
		end = str.size();
	user = str.substr(startTwo + 1, end - startTwo - 1);
	if (members.find(user) == members.end()) {
		sendToClient(c.getFd(), ERR_USERNOTINCHANNEL(c.getNick(), user, (*chan[target]).getName()));
		return ;
	}
	std::string	comment = "";
	if (end != static_cast<int>(str.length())) {
		int startThree = str.find(' ', end);
		comment = " :" + str.substr(startThree + 1, str.length() - startThree - 1);
	}
	sendToClient(c.getFd(), KICK_MSG((*chan[target]).getName(), user, comment));
	(*chan[target]).removeUserFromChan(user);
}