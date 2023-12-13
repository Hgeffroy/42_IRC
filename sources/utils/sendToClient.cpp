/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendToClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:42:35 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/13 12:05:55 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendToClient(int fd, std::string str)
{
	send(fd, str.c_str(), str.length(), MSG_DONTWAIT + MSG_NOSIGNAL); // Check les flags
	std::cout << "Sending to client: " << str;
}