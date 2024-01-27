/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:11 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/27 12:16:38 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <arpa/inet.h>
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::passCommand( int clientSocket )
{
	if (isClientFullyAuthenticated(clientSocket))
		notRegisteredReply(clientSocket, "PASS");
}

bool Server::handlePassCommand( int clientSocket, std::string command, const std::string parameters )
{
	if (this->clientStates[clientSocket].isAuthenticated == true)
		return true;
	if (command != "PASS")
		return false;
	std::string	password = parameters;
	if (password[0] == ':')
		password = parameters.substr(1, parameters.length() - 1);
	if (password != this->serverPassword)
	{
		std::string passwordMsg = ":IRCServer 464 PASS :Password incorrect\r\n";
		send(clientSocket, passwordMsg.c_str(), passwordMsg.length(), 0);
		return false;
	}
	this->clientStates[clientSocket].isAuthenticated = true;
	return true;
}
