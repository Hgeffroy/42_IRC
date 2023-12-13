/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:41:12 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 12:19:07 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

# include "replies.h"
# include "errors.h"

// Utils
std::string	getIP();
std::string getTime(Server& s);
void		printClients(std::map<std::string, Client*> map);
void		sendToClient(int fd, std::string str);

// Cmds

void	join(Server& s, Client& c, std::string& str);
void	nick(Server& s, Client& c, std::string& str);
void	pass(Server& s, Client& c, std::string& str);
void	sendMsg(Server& s, Client& c, std::string& str);
void	user(Server& s, Client& c, std::string& str);
void	mode(Server& s, Client& c, std::string& str);

void	i_opt(std::vector< Channel* >& channel, Client& c, std::string& params);

#endif

