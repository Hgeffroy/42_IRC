/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:07:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/09 10:25:55 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::vector<std::string>	splitCmd(std::string str)
{
	std::vector<std::string>	res;

	size_t	pspace = 0;
	size_t	nspace = str.find(' ');

	int i = 0;

	while (i < 10) {
		i++;
		res.push_back(str.substr(pspace, nspace - pspace + 1));
		if (nspace == std::string::npos)
			break ;
		pspace = nspace + 1;
		nspace = str.find(' ', pspace);
	}
	return (res);
}