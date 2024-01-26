/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:29:20 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/26 22:17:50 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
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

void	Server::changeClientNickname( int clientSocket, const std::string& nickname )
{
	if (!validNickname(clientSocket, nickname))
		return ;
	if (!isNicknameAvailable(nickname))
	{
		std::string	alreadyinuseMsg = ":IRCServer 433 " + nickname + " :Nickname is already in use\r\n";
		send(clientSocket, alreadyinuseMsg.c_str(), alreadyinuseMsg.length(), 0);
		return ;
	}
	std::string	nicknameChanged = ":" + this->clientStates[clientSocket].nickname
		+ " NICK " + nickname + "\r\n";
	this->clientStates[clientSocket].nickname = nickname;
	send(clientSocket, nicknameChanged.c_str(), nicknameChanged.length(), 0);
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
