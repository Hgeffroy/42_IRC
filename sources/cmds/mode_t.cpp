/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/14 10:29:01 by twang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static void	add_IOpt(Channel *channel);
static void	remove_IOpt(Channel *channel);
static void	add_KOpt(Channel *channel, std::string param);
static void	remove_KOpt(Channel *channel);
static bool	isOperator(Client &c, Channel *channel);
static bool	checkOption(std::string params);

//i: Set/remove Invite-only channel
void	i_opt(Server &s, Client &c, Channel *channel, std::string params)
{
	//client ne pourra plus rentrer dans le server sans etre invité - ajout variable bool invited dans le client
	( void )s;

//check option +k et rien derriere
	if ( !checkOption( params ) )
	{
		std::cerr << "NOT GOOD OPTION" << std::endl;
		return ;
	}
//check if client is operator
	if ( !isOperator( c, channel ) )
	{
		std::cerr << "NO OPERATOR PRIVILEGE" << std::endl;
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
void	k_opt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )s; //regarder quels elements du server peuvent servir??

//check option +k et rien derriere
	if ( !checkOption( params ) )
	{
		std::cerr << "NOT GOOD OPTION" << std::endl;
		return ;
	}
//check if client is operator
	if ( !isOperator( c, channel ) )
	{
		std::cerr << "NO OPERATOR PRIVILEGE" << std::endl;
		return ;
	}
	/* il faudrait mettre ca plus haut juste avnt de switch*/
	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			add_KOpt( channel, params );
		else if ( params[i] == '-' )
			remove_KOpt( channel );
	}
}

static void	add_IOpt(Channel *channel)
{
	if ( channel->getInviteStatus( ) )
	{
		std::cout << "Deja set connard" << std::endl;
		return ;
	}
	channel->setInviteStatus( true );
}

static void	remove_IOpt(Channel *channel)
{
	if ( !channel->getInviteStatus( ) )
	{
		std::cout << "Ce n'etais pas en invite only" << std::endl;
		return ;
	}
	channel->setInviteStatus( false );
}

static void	add_KOpt(Channel *channel, std::string param)
{
	std::string	password = getPassword( param );
	if ( password.empty( ) )
	{
		std::cout << "Invalid password" << std::endl;
		return ;
	}

	if ( channel->getKeyStatus( ) )
	{
		std::cout << "Deja set connard" << std::endl;
		return ;
	}
	channel->setKeyStatus( true );
	channel->setPassword( passwor );
	std::cout << "+k option on channel : " << channel->getName() << " is set with password : -";
	std::cout << channel->getPassword() << "-" << std::endl;
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
		channel->setPassword( password );
	}
	else
	{
		std::cout << "YOU NEED TO SET A PASSWORD MORON" << std::endl;
		return ( "" );
	}
	return ( password );
}

static void	remove_KOpt(Channel *channel)
{
	if ( !channel->getKeyStatus( ) )
	{
		std::cout << "Ce n'etais pas en invite only" << std::endl;
		return ;
	}
	channel->setKeyStatus( false );
	std::cout << "-k option on channel : " << channel->getName() << " is unset." << std::endl;
}

static bool	isOperator(Client &c, Channel *channel)
{
	std::map< std::string, std::string >members = channel->getMembers();
	if ( members[ c.getNick( ) ] != "@" )
		return ( false );
	return ( true );
}

static bool	checkOption(std::string params)
{
	std::size_t first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	std::string	option = params.substr( first_space + 1, second_space - 1 );
	std::cout << option << std::endl;
	if ( option.size() > 2 )
		return( false ) ;
	return ( true );
}
