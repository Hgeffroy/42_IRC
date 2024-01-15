/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:41:12 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 10:23:14 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

/*---- includes --------------------------------------------------------------*/

# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

# include "replies.h"
# include "errors.h"

/*---- prototypes ------------------------------------------------------------*/

// Utils
std::string					getTime(Server& s);
std::string					intostr(int a);
void						sendToClient(int fd, std::string str);
void						signals_switchman( int signal );
void						printStrVec(std::vector<std::string> v);



// Cmds
void	pass(Server& s, Client& c, std::string& str);
void	user(Server& s, Client& c, std::string& str, bool is_bot);
void	nick(Server& s, Client& c, std::string& str, bool is_bot);

void	sendMsg(Server& s, Client& c, std::string& str);
void	join(Server& s, Client& c, std::string& str);
void	mode(Server& s, Client& c, std::string& str);
void	who(Server& s, Client& c, std::string& str);
void	quit(Server& s, Client& c, std::string& str);
void	part(Server& s, Client& c, std::string& str);
void	invite( Server& s, Client& c, std::string& name );
void	topic(Server& s, Client& c, std::string& str);
void	kick(Server& s, Client& c, std::string& str);
void	motd(Server& s, Client& c);
void	pong(Server& s, Client& c, std::string str);
void	list(Server& s, Client& c, std::string str);
void	away(Server& s, Client& c, std::string& str);
void	bot( Server &s, Client& c, std::string& str );
void	moderator( Server& s, Client& c, std::string& str );


#endif

