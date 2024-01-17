/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:27:07 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/16 14:55:39 by moudrib          ###   ########.fr       */
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
	if (!parameter[nickLength] || !parameter[nickLength + 1])
		return ;
	std::string	noticeMsg = ":" + this->clientStates[clientSocket].nickname + " NOTICE " + parameter.substr(0, nickLength) + " ";
	std::string msg = parameter.substr(nickLength + 1, parameter.length() - nickLength - 1);
	if (msg.find(':') == std::string::npos)
		noticeMsg += ":";
	noticeMsg += msg + "\r\n";
	std::map<int, ClientState>::iterator	it;
	if (msg.find_first_not_of(' ') != std::string::npos)
	{	
		for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
			if (it->second.nickname == nickname)
				send(it->first, noticeMsg.c_str(), noticeMsg.length(), 0);
	}
}
