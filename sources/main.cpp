/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:57:17 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/26 17:07:54 by hgeffroy         ###   ########.fr       */
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
	}
	catch (std::exception& e)
	{
		std::cerr << "Couldn't create the server because ";
		std::cerr << e.what() << std::endl;
		return (-1);
	}


}