/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/15 17:09:14 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*---- static defines --------------------------------------------------------*/

static void			add_IOpt(Channel *channel);
static void			remove_IOpt(Channel *channel);
static void			add_KOpt(Channel *channel, std::string param);
static void			remove_KOpt(Channel *channel);
static bool			isOperator(Client &c, Channel *channel);
static bool			checkOption(std::string params);
static std::string	getPassword( std::string param );

/*----------------------------------------------------------------------------*/

//i: Set/remove Invite-only channel
void	i_opt(Client &c, Channel *channel, std::string params)
{
	if ( !checkOption( params ) )
	{
		std::cerr << PURPLE << "BAD OPTION" << END << std::endl;
		return ;
	}
	if ( !isOperator( c, channel ) )
	{
		std::cerr << PURPLE << "NO OPERATOR PRIVILEGE" << END << std::endl;
		return ;
	}

	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			add_IOpt( channel );
		else if ( params[i] == '-' )
			remove_IOpt( channel );
	}
}

//k: Set/remove the channel key (password)
void	k_opt(Client &c, Channel *channel, std::string params)
{
	if ( !checkOption( params ) )
	{
		std::cerr << PURPLE << "BAD OPTION" << END << std::endl;
		return ;
	}
	if ( !isOperator( c, channel ) )
	{
		std::cerr << PURPLE << "NO OPERATOR PRIVILEGE" << END << std::endl;
		return ;
	}
	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			add_KOpt( channel, params );
		else if ( params[i] == '-' )
			remove_KOpt( channel );
	}
}

/*----------------------------------------------------------------------------*/

static void	add_IOpt(Channel *channel)
{
	if ( channel->getInviteStatus( ) )
	{
		std::cerr << PURPLE << "+i option already set." << END << std::endl;
		return ;
	}
	channel->setInviteStatus( true );
	std::cout << YELLOW << "+i option on channel : " << channel->getName() << " is set." << END << std::endl;
}

static void	remove_IOpt(Channel *channel)
{
	if ( !channel->getInviteStatus( ) )
	{
		std::cerr << PURPLE << "+i option was not on." << END << std::endl;
		return ;
	}
	channel->setInviteStatus( false );
	std::cout << YELLOW << "-i option on channel : " << channel->getName() << " is unset successfully."  << END << std::endl;
}

/*----------------------------------------------------------------------------*/

static void	add_KOpt(Channel *channel, std::string param)
{
	std::string	password = getPassword( param );
	if ( password.empty( ) )
	{
		std::cerr << "Invalid password" << std::endl;
		return ;
	}

	if ( channel->getKeyStatus( ) )
	{
		std::cerr << PURPLE << "+k option already set." << END << std::endl;
		return ;
	}
	channel->setKeyStatus( true );
	channel->setPassword( password );
	std::cout << YELLOW << "+k option on channel : " << channel->getName() << " is set with password : -";
	std::cout << channel->getPassword() << "-" << END << std::endl;
}

static void	remove_KOpt(Channel *channel)
{
	if ( !channel->getKeyStatus( ) )
	{
		std::cerr << PURPLE << "+k option was not on." << END << std::endl;
		return ;
	}
	channel->setKeyStatus( false );
	std::cout << YELLOW << "-k option on channel : " << channel->getName() << " is unset successfully." << END << std::endl;
}

/*----------------------------------------------------------------------------*/

static bool	isOperator(Client &c, Channel *channel)
{
	std::map< std::string, std::string >members = channel->getMembers();
	if ( members[ c.getNick( ) ] != "@" && members[ c.getNick( ) ] != "~" )
		return ( false );
	return ( true );
}

static bool	checkOption(std::string params)
{
	std::size_t first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	std::string	option = params.substr( first_space + 1, second_space - 1 );
	if ( option.size() > 2 )
		return( false ) ;
	return ( true );
}

static std::string	getPassword( std::string param )
{
	std::string	password;
	std::size_t	first_space = param.find( ' ' );
	std::size_t	second_space = param.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		password = param.substr( second_space + 1 );
		if ( password.empty() )
			return ( "" );
	}
	else
		return ( "" );

	return ( password );
}
