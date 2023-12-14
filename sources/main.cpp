/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:57:17 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/14 09:21:16 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	test(int sig)
{
	std::cout << "Alloooooo" << std::endl;
	if (sig == SIGINT)
		throw	std::exception();
}

int	main(int ac, char** av)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &test);

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
		std::cerr << "Couldn't create the server because ";
		std::cerr << e.what() << std::endl;
		// Liberer le port !!
		return (-1);
	}

}