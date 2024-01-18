/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:33:47 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/16 14:21:46 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <iostream>

# define INVALID_ARGUMENTS		BOLD FG_RED "Error: " FG_WHITE "invalid arguments."
# define VALID_INPUT			"Try ./ft_irc + \"Port number\" + \"Password\""

# define INVALID_PORT_NUMBER	BOLD FG_RED "Error: " FG_WHITE "invalid port number."
# define VALID_PORT_NUMBER		"The port number must be between 1024 and 65535"

# define SOCKET_CREATION		BOLD FG_RED "Error: creating socket"

# define SOCKET_BINDING			BOLD FG_RED "Error: binding socket"

# define LISTENING_ERROR		BOLD FG_RED "Error: listening for connections"

# define ACCEPT_CONNECTION		BOLD FG_RED "Error: accepting connection"

# define POLL_FAILURE			BOLD FG_RED "Error: poll() failed"

# define SETSOCKOPT				BOLD FG_RED "Error: setsockopt() failed"

bool	setNonBlocking(int fd);
bool	validCommands( const std::string& command );
bool	validNickname( int clientSocket, const std::string& nickname );
std::string	getCommand( int clientSocket, const std::string& message );
std::string	getParameters( int clientSocket, const std::string& command, const std::string& message );
// dyawli maytems7och fl merg =====================================================================
void sendwrongCommandMessage(int clientSocket);
std::string removeMsgCommand(const std::string& fullMessage);
// void sendwrongUserMessage(int clientSocket, std::string& nickname);
//=================================================================================================

#endif