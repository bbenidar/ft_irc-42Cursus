/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:24:51 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 15:01:57 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <sys/socket.h>
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

bool Server::isClientFullyAuthenticated(int clientSocket) {
	return clientStates[clientSocket].isAuthenticated &&
			clientStates[clientSocket].hasNick &&
			clientStates[clientSocket].hasUser;
}

bool Server::handlePassCommand( int clientSocket, const std::string& message )
{
	if (this->clientStates[clientSocket].isAuthenticated == true)
		return true;
	if (message.substr(0, 4) != "PASS")
		return false;
	if (message.substr(5, message.length() - 6) != this->serverPassword)
	{
		send(clientSocket, FG_RED "   Wrong password\n" FG_DEFAULT, 28 , 0);
		return false;
	}

	this->clientStates[clientSocket].isAuthenticated = true;
	return true;
}

bool Server::handleNickCommand( int clientSocket, const std::string& message )
{
	if (this->clientStates[clientSocket].hasNick == true)
		return true;
	std::string	nickname = message.substr(5, message.length() - 6);
	if (message.substr(0, 4) != "NICK" || nickname.length() == 0)
		return false;
	this->clientStates[clientSocket].hasNick = true;
	this->clientStates[clientSocket].nickname = nickname;
	return true;
}

bool Server::handleUserCommand( int clientSocket, const std::string& message )
{
	if (this->clientStates[clientSocket].hasUser == true)
		return true;
	if (message.substr(0, 4) != "USER")
		return false;
	size_t	i = 1, pos = 8; // "USER " + 3 spaces
	std::string	word;
	std::stringstream	input(message.substr(5, message.length() - 6));

	while (getline(input, word, ' '))
	{
		if (i == 1)
			this->clientStates[clientSocket].username = word;
		if (i == 2)
			this->clientStates[clientSocket].hostname = word;
		if (i == 3)
		{
			this->clientStates[clientSocket].servername = word;
			pos += word.length();
			break ;
		}
		i++;
		pos += word.length();
	}
	if (message.find(":", pos) == pos)
	{
		this->clientStates[clientSocket].realname = message.substr(pos + 1, message.length() - pos - 2);
		this->clientStates[clientSocket].hasUser = true;
		send(clientSocket, BOLD FG_GREEN "authentication succeeded\n" FG_DEFAULT, 39, 0);
		return true;
	}
	return false;
}
