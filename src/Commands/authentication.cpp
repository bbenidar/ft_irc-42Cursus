/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 10:24:51 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/04 21:24:01 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <sys/socket.h>
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

bool Server::isClientFullyAuthenticated(int clientSocket) {
	return clientStates[clientSocket].isAuthenticated &&
			clientStates[clientSocket].hasNick &&
			clientStates[clientSocket].hasUser;
}

bool Server::handlePassCommand( int clientSocket, const std::string& command, const std::string& parameters )
{
	if (this->clientStates[clientSocket].isAuthenticated == true)
		return true;
	if (command != "PASS")
		return false;
	std::string	password = parameters;
	if (password != this->serverPassword)
	{
	    std::string passwordMsg = ":IRCServer 464 PASS :Password incorrect\r\n";
		send(clientSocket, passwordMsg.c_str(), passwordMsg.length(), 0);
		return false;
	}
	this->clientStates[clientSocket].isAuthenticated = true;
	return true;
}

bool Server::handleNickCommand( int clientSocket, const std::string& command, const std::string& parameters )
{
	if (this->clientStates[clientSocket].hasNick == true)
		return true;
	std::string	nickname = parameters;
	if (command != "NICK" || nickname.length() == 0)
		return false;
	if (!isNicknameAvailable(nickname))
	{
		send(clientSocket, FG_RED "   That nickname is taken. Try another.\n" FG_DEFAULT, 50 , 0);
		return false;
	}
	this->clientStates[clientSocket].hasNick = true;
	this->clientStates[clientSocket].nickname = nickname;
	return true;
}

bool Server::handleUserCommand( int clientSocket, const std::string& command, const std::string& parameters )
{
	if (this->clientStates[clientSocket].hasUser == true)
		return true;
	if (command != "USER")
		return false;
	size_t	i = 1, pos = 3;
	std::string	word;
	std::stringstream	input(parameters);

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
	if (parameters.find(":", pos) == pos)
	{
		this->clientStates[clientSocket].realname = parameters.substr(pos + 1, parameters.length() - pos - 2);
		this->clientStates[clientSocket].hasUser = true;
		return true;
	}
	return false;
}
