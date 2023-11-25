/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:14:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/11/25 12:32:45 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	printTab(long* tab, int size)
{
	for (int i = 0; i < size; i++)
		std::cout << "tab[" << i << "] = " << tab[i] <<std::endl;
}

void	checkPort(std::string portstr)
{
	long	port[4];
	int		start; // Sur le premier nombre a check
	int		end = -1; // Sur le prochain '.'

	for (int i = 0; i < 4; ++i)
	{
		start = end + 1;
		end = static_cast<int>(portstr.find('.', start));
		if ((end == static_cast<int>(std::string::npos) && i != 3) || (i == 3 && end != static_cast<int>(std::string::npos)))
			throw std::exception();
		else if (i == 3)
			end = static_cast<int>(portstr.size());
		port[i] = std::strtol(portstr.substr(start, end - start).c_str(), NULL, 10);
		if (errno == ERANGE || port[i] < 0 || port[i] > 255)
			throw std::exception();
	}
	printTab(port, 4);
}

void	checkPassword(std::string pass)
{
	if (pass.size() < 5)
		throw std::exception();

	std::string::iterator	it;
	for (it = pass.begin(); it != pass.end(); ++it)
		if (!std::isprint(*it) || *it == ' ' || *it == '	')
			std::cout << *it << std::endl;
}

void	checkArgs(int ac, char **av)
{
	if (ac != 3)
		throw std::exception();

	checkPort(av[1]);
	checkPassword(av[2]);
}
