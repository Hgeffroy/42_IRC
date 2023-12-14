/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeffroy <hgeffroy@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 08:39:59 by hgeffroy          #+#    #+#             */
/*   Updated: 2023/12/10 09:42:12 by hgeffroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.h"

class	Client;

class	Channel
{

private:

	const std::string					_name;
	std::map<std::string, std::string>	_members;
	std::string 						_topic;
	int									_userLimit;
	int									_nbUsers;

	Channel();

public:

	Channel(std::string const& name, std::string const& founder);
	~Channel();

	std::string 						getName() const;
	std::string 						getTopic() const;
	std::map<std::string, std::string>	getMembers() const;
	int									getNbUsers() const;
	int									getUserLimit() const;
	void								setUserLimit( int lim );

	void	addUser(Client& newClient);

};

#endif