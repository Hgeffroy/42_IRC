/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:57:17 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/25 09:10:39 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	main(int ac, char **av)
{
	try
	{
		checkArgs(ac, av);
	}
	catch (std::exception& e)
	{
		std::cerr << "Couldn't create the server because ";
		std::cerr << e.what() << std::endl;
	}


//	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
}