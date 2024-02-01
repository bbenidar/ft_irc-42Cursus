/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 19:38:30 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/31 16:09:44 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"


void Server::handleInvitechannel(const std::string& msge, int clientSocket)
{
	std::string clientName = removeMsgCommand(msge);
	if (clientName.size() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string channel = msge.substr(msge.find(clientName) + clientName.length());
	size_t	begin = channel.find_first_not_of(" \n\r", 0);
	size_t	end = channel.find_last_not_of(" \n\r", channel.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	channel =  channel.substr(begin, end - begin + 1);
	std::vector<std::string> clientNames = split(clientName, ',');
	std::map<std::string, Channels>::iterator	it2;
	for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
		if (it2->first == channel)
			break ;
	if (it2 == this->channels.end())
		return noSuchChannelReply(clientSocket, channel, "INVITE " + this->clientStates[clientSocket].nickname + " ");
	for (size_t i = 0; i < clientNames.size(); i++)
	{
		std::map<int, ClientState>::iterator	it;
		for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
			if (it->second.nickname == clientNames[i])
				break ;
		if (it == this->clientStates.end())
			return noSuchNickChannelReply(clientSocket, clientNames[i]);
		if (it2->second.getifClientIsBanned(it->first))
		{
			std::string	reply = ":IRCServer 404 " + channel + " " + clientNames[i] + " :Cannot send to channel\r\n";
			send(clientSocket, reply.c_str(), reply.length(), 0);
			return ;
		}
		std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
		if (tmp.count(clientSocket)== 0)
			return notOnThatChannel(clientSocket, channel);
		if (it2->second.getifClientIsModerator(clientSocket) == false)
			return notChannelOperatorReply(clientSocket, channel);
		if (tmp.count(it->first))
			return alreadyOnChannel(clientSocket, channel, " " + clientNames[i]);
		it2->second.setChannelInvitedClients(it->first, it->second);
		std::string	inviteMsg = ":IRCServer 341 " + channel + " " + clientNames[i] + " " + channel + "\r\n";
		send(clientSocket, inviteMsg.c_str(), inviteMsg.length(), 0);
	}
}
