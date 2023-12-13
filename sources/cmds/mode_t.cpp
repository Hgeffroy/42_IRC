/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wangthea <wangthea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/13 20:03:26 by wangthea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static void	add_IOpt(Server &s, Client &c, Channel *channel, std::string params);
static void	remove_IOpt(Server &s, Client &c, Channel *channel, std::string params);
static void	add_KOpt(Server &s, Client &c, Channel *channel, std::string params);
static void	remove_KOpt(Server &s, Client &c, Channel *channel, std::string params);

//i: Set/remove Invite-only channel
void	i_opt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode
	//client ne pourra plus rentrer dans le server sans etre invité - ajout variable bool invited dans le client
	( void )s;
	std::size_t first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	std::string	option = params.substr( first_space + 1, second_space );
	std::cout << option << std::endl;
	if ( option.size() > 2 )
		std::cout << "WHAT'S YOUR PROBLEM" << std::endl;
	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			add_IOpt( s, c, channel, params );
		else if ( params[i] == '-' )
			remove_IOpt( s, c, channel, params );
	}
}

//k: Set/remove the channel key (password)
void	k_opt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s; //regarder quels elements du server peuvent servir??
	std::size_t first_space = params.find( ' ' );
	std::size_t second_space = params.find( ' ', first_space + 1 );
	std::string	option = params.substr( first_space + 1, second_space );
	std::cout << option << std::endl;
	if ( option.size() > 2 )
		std::cout << "WHAT'S YOUR PROBLEM" << std::endl;
	/* il faudrait mettre ca plus haut juste avnt de switch*/
	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			add_KOpt( s, c, channel, params );
		else if ( params[i] == '-' )
			remove_KOpt( s, c, channel, params );
	}
}

static void	add_IOpt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s;
	( void )params;
	if ( channel->getInviteStatus( ) )
	{
		std::cout << "Deja set connard" << std::endl;
		return ;
	}
	channel->setInviteStatus( true );
}

static void	remove_IOpt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s;
	( void )params;
	if ( !channel->getInviteStatus( ) )
	{
		std::cout << "Ce n'etais pas en invite only" << std::endl;
		return ;
	}
	channel->setInviteStatus( false );
}

static void	add_KOpt(Server &s, Client &c, Channel *channel, std::string param)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s;
	std::size_t first_space = param.find( ' ' );
	std::size_t second_space = param.find( ' ', first_space + 1 );
	if ( second_space != std::string::npos )
	{
		std::string password = param.substr( second_space + 1 );
		std::cout << "Mot de passe : -" << password << "-" << std::endl;
		channel->setPassword( password );
		std::cout << YELLOW << channel->getPassword() << END << std::endl;
	}
	else
		std::cout << "YOU NEED TO SET A PASSWORD MORON" << std::endl;
	
	if ( channel->getKeyStatus( ) )
	{
		std::cout << "Deja set connard" << std::endl;
		return ;
	}
	channel->setKeyStatus( true );
	channel->setPassword( "NON" );
}

static void	remove_KOpt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s;
	std::cout << params << std::endl;
	if ( !channel->getKeyStatus( ) )
	{
		std::cout << "Ce n'etais pas en invite only" << std::endl;
		return ;
	}
	channel->setKeyStatus( false );
	//vider le password ?? inutile il suffit de l'ignorer et de l'ecraser apres le nouveau.
}
