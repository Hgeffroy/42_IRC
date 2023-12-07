/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getIP.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:32:32 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/06 10:40:52 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	getIP()
{
	char szBuffer[1024];

	struct hostent *host = gethostbyname(szBuffer);
	if (!host)
		return ("");
	return (host->h_name);
}