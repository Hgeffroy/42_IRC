/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 11:00:13 by twang             #+#    #+#             */
/*   Updated: 2024/01/05 15:36:07 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_H
# define BOT_H

/*---- librairies ------------------------------------------------------------*/

# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <string>
# include <exception>
# include <cerrno>
# include <vector>
# include <map>
# include <algorithm>
# include <unistd.h>
# include <signal.h>
# include <netdb.h>
# include <sstream>
# include <stdlib.h>
# include <cctype>
# include <cstdio>
# include <ios>

/*---- includes --------------------------------------------------------------*/
/*---- colors ----------------------------------------------------------------*/

# define BLUE		"\001\033[1;34m\002"
# define RED		"\001\033[1;31m\002"
# define YELLOW		"\001\033[1;33m\002"
# define GREEN		"\001\033[1;32m\002"
# define PURPLE		"\001\033[1;35m\002"
# define GREY		"\001\033[1;30m\002"

# define END		"\001\033[0m\002"
# define BOLD		"\001\033[1m\002"
# define ITALIC		"\001\033[3m\002"
# define UNDERLINE	"\001\033[4m\002"
# define DEL_LINE	"\001\033[2K\r\002"

#endif