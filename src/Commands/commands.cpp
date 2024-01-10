/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/10 22:24:02 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void	Server::notice( int clientSocket, const std::string& parameter )
{
	std::string			nickname;
	std::stringstream	parameters(parameter);
	getline(parameters, nickname, ' ');
	while (nickname.length() == 0)
	{
		getline(parameters, nickname, ' ');		
		if (parameters.eof())
			break ;
	}
	size_t	nickLength = nickname.length();
	std::string	noticeMsg = ":" + this->clientStates[clientSocket].nickname + " NOTICE " + parameter.substr(0, nickLength) + " ";
	if (parameter.find(' ') != std::string::npos)
		noticeMsg += ":";
	if (!parameter[nickLength] || !parameter[nickLength + 1])
		return ;
	noticeMsg += parameter.substr(nickLength + 1, parameter.length() - nickLength - 1) + "\r\n";
	std::map<int, ClientState>::iterator	it;
	for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
	{
		if (it->second.nickname == nickname)
			send(it->first, noticeMsg.c_str(), noticeMsg.length(), 0);
	}
}

void	Server::processAuthenticatedClientCommand(int clientSocket, const std::string& command, const std::string& parameters )
{
	int cmd	=	command == "PASS" ? 1 :
				command == "NICK" ? 2 :
				command == "JOIN" ? 3 :
				command == "PRIVMSG" ? 4 :
				command == "NOTICE" ? 5 : 6;
	switch (cmd)
	{
		case 1:
			passCommand(clientSocket);
			break;
		case 2:
			changeClientNickname(clientSocket, parameters);
			break;
		case 3:
			handelJoinchannel(command + " " + parameters, clientSocket);
			break;
		case 4:
			send_message(command + " " + parameters, clientSocket);
			break;
		case 5:
			notice(clientSocket, parameters);
			break;
		default:
			break;
	}
}