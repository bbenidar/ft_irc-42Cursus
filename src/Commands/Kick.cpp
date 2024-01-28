/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:48:36 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/28 20:55:24 by bbenidar         ###   ########.fr       */
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
	//kick => make the reason optional 
	for (int i = 0; i < (int)Chans.size(); i++)
	{
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
				return noSuchChannelReply(clientSocket, Chans[i], "KICK " + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, Chans[i]);
			std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
			if (tmp.count(it->first) == 0)
				return theyNotOnThatChannel(clientSocket, Nams[j], Chans[i]);
			this->channels[Chans[i]].KickClient(it->first);
			this->channels[Chans[i]].sendBroadcastMessage(":" + clientStates[clientSocket].nickname + " KICK " + Chans[i] + " " + it->second.nickname + "\r\n", clientSocket);
			if (this->channels[Chans[i]].getChannelClients().size() == 0)
			{
				this->channels.erase(Chans[i]);
				return ;
			}
			if (this->channels[Chans[i]].getifClientIsModerator(it->first))
			{
				this->channels[Chans[i]].removeModerator(it->first);
				std::map<int, std::vector<ClientState> > tmp = this->channels.begin()->second.getChannelClients();
				this->channels[Chans[i]].setChannelModerators(tmp.begin()->first, tmp.begin()->second);
			}
			
			
		}
	}
}