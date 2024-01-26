/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:29:20 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/20 20:50:02 by moudrib          ###   ########.fr       */
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
