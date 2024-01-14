/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:02:32 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/14 12:29:34 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

# include <iostream>
#include "../include/utils/colors.hpp"

# define BUFFER_SIZE 4096

# define INVALID_ARGUMENTS		BOLD FG_RED "Error: " FG_WHITE "invalid arguments."
# define VALID_INPUT			"Try ./askme + \"Port number\" + \"Password\" + \"Nickname\""

# define INVALID_PORT_NUMBER	BOLD FG_RED "Error: " FG_WHITE "invalid port number."
# define VALID_PORT_NUMBER		"The port number must be between 1024 and 65535"

# define INVALID_PASSWORD		BOLD FG_RED "Error: " FG_WHITE "password incorrect."

# define INVALID_NICKNAME		BOLD FG_RED "Error: " FG_WHITE "invalid nickname."

# define SOCKET_CREATION		BOLD FG_RED "Error: creating socket"

# define SOCKET_BINDING			BOLD FG_RED "Error: binding socket"

# define CONNECTION_FAILED		BOLD FG_RED "Error: Failed to connect to server"

class Bot
{
private:
	int				botSocket;
	unsigned short	port;
	bool			isAuthenticated;
	std::string		server;
	std::string		sender;
	std::string		command;
	std::string		receiver;
	std::string		nickname;
	std::string		serverPassword;

public:

	void	connectToServer();
	void	respondToMessages();
	void	botAuthentication();
	void	handleCommands( const std::string& message );
	void	failedAuthenticationCheck( std::string& message );
	void	sendMessage(const std::string& nickname, const std::string& msg);
	void	parseArguments( const std::string& s_port, const std::string& password, const std::string& nickname );
};

#endif