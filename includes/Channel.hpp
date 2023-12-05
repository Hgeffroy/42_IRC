/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:39:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/05 15:15:28 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.h"

class	Client;

class	Channel
{

private:

	const std::string	_name;
	std::vector<Client>	_members;
	std::vector<Client>	_ops;

	Channel();

public:

	Channel(std::string& name, Client& op);
	~Channel();

	std::string getName() const;

	void	addUser(Client& newClient);

};

#endif