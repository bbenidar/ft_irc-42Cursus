/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:48:36 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/27 12:46:20 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

void Server::handleKickCommand(const std::string& msge, int clientSocket)
{
	(void)clientSocket;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.size() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string Nickname = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Nickname.find_first_not_of(" \n\r", 0);
	size_t	end = Nickname.find_first_of(" \n\r", begin);
	if (begin == std::string::npos || end == std::string::npos)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	Nickname =  Nickname.substr(begin, end - begin);
	std::vector<std::string> Nams = split(Nickname, ',');
	std::vector<std::string> Chans = split(chanName, ',');
	
	for (int i = 0; i < (int)Chans.size(); i++)
	{
		if (Chans[i].length() > 0 && Chans[i][0] == '#')
			Chans[i].erase(0, 1);
		for (int j = 0; j < (int)Nams.size(); j++)
		{
			
			std::map<int, ClientState>::iterator	it;
			for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
				if (it->second.nickname == Nams[j])
					break ;
			if (it == this->clientStates.end())
				return noSuchNickChannelReply(clientSocket, Nams[j]);
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == Chans[i])
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, Chans[i]);
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, Chans[i]);
			std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
			if (tmp.count(it->first) == 0)
				return theyNotOnThatChannel(clientSocket, Nams[j], Chans[i]);
			this->channels[Chans[i]].KickClient(it->first);
		}
	}
}