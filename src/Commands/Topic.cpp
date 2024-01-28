/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 10:54:18 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/28 15:41:51 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

void Server::handleTopicCommand(const std::string& msge, int clientSocket)
{
	(void)clientSocket;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.length() == 0)
		return notEnoughParametersReply(clientSocket, "TOPIC");
	std::map<std::string, Channels>::iterator	it2;
	for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
		if (it2->first == chanName)
			break ;
	if (it2 == this->channels.end())
		return noSuchChannelReply(clientSocket, chanName, "TOPIC " + this->clientStates[clientSocket].nickname + " ");
	std::string Topic = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Topic.find_first_not_of(" \n\r", 0);
	size_t	end = Topic.find_last_not_of(" \n\r", Topic.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
	{
		if (it2->second.getChannelTopic() == "")
		{
			std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + chanName + " :No topic is set\r\n" ;
			send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
			topicRep = ":IRCServer 333 " + this->clientStates[clientSocket].nickname + " " + chanName + " " + this->clientStates[clientSocket].nickname + " " + channels[chanName].getTopicTime() + "\r\n" ;
			send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
			return ;
		}
		else
		{
			std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + chanName + " " + channels[chanName].getChannelTopic() + "\r\n";
			send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
			topicRep = ":IRCServer 333 " + this->clientStates[clientSocket].nickname + " " + chanName + " " + this->clientStates[clientSocket].nickname + " " + channels[chanName].getTopicTime() + "\r\n" ;
			send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
			return ;
		}
	}
	Topic =  Topic.substr(begin, end - begin + 1);
	std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
	if (tmp.count(clientSocket)== 0)
		return notOnThatChannel(clientSocket, chanName);
	if (it2->second.getChannelTopicModeratorOnly() == true && it2->second.getifClientIsModerator(clientSocket) == false)
		return notChannelOperatorReply(clientSocket, chanName);
	it2->second.setChannelTopic(Topic);
	std::time_t currentTime = std::time(0);
	std::string time = std::ctime(&currentTime);
    this->channels[chanName].setTopicTime(time);
	std::string brdcstMsg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " TOPIC " + chanName + " :"+ channels[chanName].getChannelTopic() +"\r\n";
	this->channels[chanName].sendBroadcastMessage(brdcstMsg, clientSocket);
}
