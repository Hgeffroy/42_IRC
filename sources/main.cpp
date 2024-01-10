/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:57:17 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/10 16:01:42 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	main(int ac, char** av)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGINT, &signals_switchman);

	if (ac != 3)
	{
		std::cout << "Wrong usage:" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (-1);
	}

	try
	{
		Server	serv(av[1], av[2]);
		while (true)
		{
			serv.initFd();
			serv.checkFd();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Server closed: ";
		std::cerr << e.what() << std::endl;
		return (-1);
	}
	//une fin de ligne doit imperativement etre /n/r		on ne doit pas gerer un nc localhost 2000 sans le -C
}