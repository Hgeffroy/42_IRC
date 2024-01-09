/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:07:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/08 15:34:43 by hgeffroy         ###   ########.fr       */
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
		res.push_back(str.substr(pspace, nspace));
		if (nspace == std::string::npos)
			break ;
		pspace = nspace + 1;
		nspace = str.find(' ', pspace);
	}
	return (res);
}