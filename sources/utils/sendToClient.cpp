/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendToClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 08:42:35 by hgeffroy          #+#    #+#             */
/*   Updated: 2024/01/15 09:45:22 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	sendToClient(int fd, std::string str)
{
	send(fd, str.c_str(), str.length(), MSG_DONTWAIT | MSG_NOSIGNAL);
	std::cout << BLUE << "To " << fd << ": " << str << END;
}