/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:24:51 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/16 13:57:14 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <sys/socket.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

bool Server::isNicknameAvailable( const std::string& newNickname )
{
	std::map<int, ClientState>::iterator	it;
	for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
	{
		if (it->second.nickname == newNickname)
			return false;
	}
	return true;
}

bool Server::isClientFullyAuthenticated(int clientSocket)
{
	return clientStates[clientSocket].isAuthenticated &&
			clientStates[clientSocket].hasNick &&
			clientStates[clientSocket].hasUser;
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

bool Server::handleNickCommand( int clientSocket, std::string command, const std::string parameters )
{
	if (this->clientStates[clientSocket].hasNick == true)
		return true;
	if (command != "NICK")
		return false;
	std::string	nickname = parameters;
	if (nickname[0] == ':')
		nickname = parameters.substr(1, parameters.length() - 1);
	if (!validNickname(clientSocket, nickname))
		return false;
	if (!isNicknameAvailable(nickname))
	{
		std::string	alreadyinuseMsg = ":IRCServer 433 " + nickname + " :Nickname is already in use\r\n";
		send(clientSocket, alreadyinuseMsg.c_str(), alreadyinuseMsg.length(), 0);
		return false;
	}
	this->clientStates[clientSocket].hasNick = true;
	this->clientStates[clientSocket].nickname = nickname;
	return true;
}

bool Server::handleUserCommand( int clientSocket, std::string command, const std::string parameters )
{
	if (this->clientStates[clientSocket].hasUser == true)
		return true;
	if (command != "USER")
		return false;
	size_t	i = 0, pos = 3;
	std::string	word;
	std::stringstream	input(parameters);

	while (!input.eof())
	{
		getline(input, word, ' ');
		if (word.length() > 0)
		{
			i++;
			if (i == 1)
				this->clientStates[clientSocket].username = word;
			else if (i == 2)
				this->clientStates[clientSocket].hostname = word;
			else if (i == 3)
				this->clientStates[clientSocket].servername = word;
			else if (i == 4)
			{
				pos = parameters.find(word) + 1;
				this->clientStates[clientSocket].realname = (word[0] == ':') ? parameters.substr(pos, parameters.length() - pos) : word;
				break ;
			}
		}
	}
	if (i != 4)
	{
		std::string    notEnoughMsg = ":IRCServer 461 " + command + " :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return false;
	}
	this->clientStates[clientSocket].hasUser = true;
	return true;
}
