/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getIP.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:32:32 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/18 13:10:15 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	getIP() //getsockname ?
{
	char szBuffer[1024] = "192.168.122.1";

	struct hostent *host = gethostbyname(szBuffer);
	if (!host)
		return ("localhost");
	return (host->h_name);
}