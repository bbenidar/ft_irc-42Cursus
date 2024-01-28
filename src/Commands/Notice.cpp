/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 14:27:07 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 17:20:28 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void	Server::handleNoticeCommand( int clientSocket, const std::string& parameter )
{
	std::string			nickname,nicknames;
	std::stringstream	parameters(parameter);
	while (nicknames.length() == 0)
	{
		getline(parameters, nicknames, ' ');		
		if (parameters.eof())
			break ;
	}
	std::stringstream	ss(nicknames);
	std::map<std::string, std::string>	nicknamesMap;

	while (!ss.eof())
	{
		getline(ss, nickname, ',');
		if (nickname.length() > 0)
			nicknamesMap.insert(std::pair<std::string, std::string>(nickname, ""));
	}
	size_t	nickLength = nicknames.length();
	if (!parameter[nickLength] || !parameter[nickLength + 1])
		return ;
	std::map<std::string, std::string>::iterator	it;
	for (it = nicknamesMap.begin(); it != nicknamesMap.end(); it++)
	{
		std::string	noticeMsg = ":" + this->clientStates[clientSocket].nickname + " NOTICE " + it->first + " ";
		std::string msg = parameter.substr(nickLength + 1, parameter.length() - nickLength - 1);
		if (msg.find(':') == std::string::npos)
			noticeMsg += ":";
		noticeMsg += msg + "\r\n";
		std::map<int, ClientState>::iterator	ite;
		if (msg.find_first_not_of(' ') != std::string::npos)
		{	
			for (ite = this->clientStates.begin(); ite != this->clientStates.end(); ite++)
				if (ite->second.nickname == it->first)
					send(ite->first, noticeMsg.c_str(), noticeMsg.length(), 0);
		}
	}
}
