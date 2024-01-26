/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:11 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/26 22:18:16 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void	Server::passCommand( int clientSocket )
{
	if (isClientFullyAuthenticated(clientSocket))
	{
		std::string mayNotRegisterMsg = ":IRCServer 462 PASS :You may not reregister\r\n";
		send(clientSocket, mayNotRegisterMsg.c_str(), mayNotRegisterMsg.length(), 0);
		return ;
	}
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
