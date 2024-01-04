#ifndef BOT_CPP
# define BOT_CPP

# include "bot.h"

class Bot
{
private:
	std::string	_addr;
	int			_port;
	std::string	_passwd;
	std::string	_nick;
	std::string	_user;
	int			_socket;
public:
	Bot();
	~Bot();
};

#endif