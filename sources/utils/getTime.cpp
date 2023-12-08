/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getTime.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:21:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/06 12:28:51 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	getTime(Server &s)
{
	struct tm	tstruct;
	char 		buffer[16];

	tstruct = *localtime(s.getCreationTime());
	strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &tstruct);

	return (buffer);
}