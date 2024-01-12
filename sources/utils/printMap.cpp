/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printMap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 14:28:15 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/11 14:28:15 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	printClients(std::map<std::string, Client*> map)
{
	std::map<std::string, Client*>::iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		std::cout << "First: " << it->first << " Second: " << it->second->getNick() << std::endl;
	}
}

void	printStrVec(std::vector<std::string> v)
{
	std::vector<std::string>::iterator it;
	int i = 0;

	for (it = v.begin(); it != v.end(); ++it) {
		std::cout << "v[" << i << "] = -" << *it << "-" << std::endl;
		i++;
	}
}