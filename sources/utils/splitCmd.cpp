/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:07:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/09 10:35:24 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::vector<std::string>	splitCmd(std::string str, char type)
{
	std::vector<std::string>	res;

	size_t	pspace = 0;
	size_t	nspace = str.find(type);

	while (true) {
		res.push_back(str.substr(pspace, nspace - pspace + 1));
		if (nspace == std::string::npos)
			break ;
		pspace = nspace + 1;
		nspace = str.find(type, pspace);
	}
	return (res);
}