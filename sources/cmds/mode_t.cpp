/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_t.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twang <twang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 09:39:11 by twang             #+#    #+#             */
/*   Updated: 2023/12/13 16:33:16 by twang            ###   ########.fr       */
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
	( void )s;
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
	std::cout << BLUE << channel->getName() << END << std::endl;
	std::cout << YELLOW << params << END << std::endl;
	for( std::size_t i = 0; i < params.size(); i++ )
	{
		if ( params[i] == '+' )
			//si je n'ai rien apres la lettre -> IL FAUT UN MOT DE PASSE
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

static void	add_KOpt(Server &s, Client &c, Channel *channel, std::string params)
{
	( void )c; //verifier si le client a les droits pour ajouter un mode 
	( void )s;
	std::cout << params << std::endl;
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
