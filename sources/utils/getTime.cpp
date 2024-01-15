/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getTime.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:21:33 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 09:46:25 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	getTime(Server &s)
{
	struct tm	tstruct;
	char		buffer[16];

	tstruct = *localtime(s.getCreationTime());
	strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", &tstruct);

	return (buffer);
}