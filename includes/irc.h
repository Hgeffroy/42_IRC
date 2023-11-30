/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 13:43:32 by twang             #+#    #+#             */
/*   Updated: 2023/11/27 13:46:28 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef IRC_H
# define IRC_H

# include <iostream>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdlib>
# include <string>
# include <exception>
# include <cerrno>
# include <vector>
# include <algorithm>

# include "defines.h"
# include "enum.h"

# include <cctype> // Pas inclus dans cstdlib ?

#endif