/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:57:17 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 10:27:08 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	main(int ac, char** av)
{

	if (ac != 3)
	{
		std::cout << "Wrong usage:" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
		return (-1);
	}

	try
	{
		Server	serv(av[1], av[2]);
		while (1)
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