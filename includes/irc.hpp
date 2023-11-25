/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:41:12 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/25 11:52:52 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <sys/socket.h>
# include <cstdlib>
# include <string>
# include <exception>
# include <cerrno>

# include <cctype> // Pas inclus dans cstdlib ?

# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"

void	checkArgs(int ac, char **av);

#endif

