/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:07:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/08 14:15:52 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::vector<std::string>	splitCmd(std::string str)
{
	std::vector<std::string>	res;

	size_t	pspace = 0;
	size_t	nspace = str.find(' ');
	while (pspace != std::string::npos) {
		std::string	tmp = str.substr(pspace, nspace - 1);
		
	}
}