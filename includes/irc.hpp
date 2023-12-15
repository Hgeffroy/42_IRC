/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:41:12 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/15 09:58:41 by hgeffroy         ###   ########.fr       */
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
# include <sstream>

/*---- prototypes ------------------------------------------------------------*/

// Utils
std::string	getIP();
std::string getTime(Server& s);
void		sendToClient(int fd, std::string str);
void		printClients(std::map<std::string, Client*> map);

// Cmds
void	join(Server& s, Client& c, std::string& str);
void	nick(Server& s, Client& c, std::string& str);
void	pass(Server& s, Client& c, std::string& str);
void	sendMsg(Server& s, Client& c, std::string& str);
void	user(Server& s, Client& c, std::string& str);
void	mode(Server& s, Client& c, std::string& str);
void	quit(Server& s, Client& c, std::string& str);
void	part(Server& s, Client& c, std::string& str);

void	i_opt(Client &c, Channel *channel, std::string params);
void	k_opt(Client &c, Channel *channel, std::string params);

void	who(Server& s, Client& c, std::string& str);

#endif

