/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:13:28 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/29 18:06:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"

std::vector<std::string> removeDpicates(std::vector<std::string> channel)
{
	std::vector<std::string> tmp;
	for (size_t i = 0; i < channel.size(); i++)
	{
		for (size_t j = 0; j < tmp.size(); j++)
		{
			if (channel[i] == tmp[j])
				break ;
			if (j == tmp.size() - 1)
				tmp.push_back(channel[i]);
		}
		if (tmp.size() == 0)
			tmp.push_back(channel[i]);
	}
	return (tmp);
}


void Server::send_message(const std::string& msge, int clientSocket)
{
	std::string channels = removeMsgCommand(msge);
	if (channels.size() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string message = msge.substr(msge.find(channels) + channels.length());
	size_t	begin = message.find_first_not_of(" \n\r", 0);
	size_t	end = message.find_last_not_of(" \n\r", message.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
		return noMessageToSend(clientSocket);
	message =  message.substr(begin, end - begin + 1);
	std::vector<std::string> channel;
	channel = split(channels, ',');
	channel = removeDpicates(channel);
	if (message[0] == ':')
		message.erase(0,1);
	for (size_t i = 0; i < channel.size(); i++)
	{
		if (channel[i].at(0) == '#')
		{
			if (this->channels.count(channel[i]) == 0)
				return noSuchChannelReply(clientSocket, channel[i], "PRIVMSG ");
			std::map<int, std::vector<ClientState> > tmp = this->channels[channel[i]].getChannelClients();
			std::map<int, std::vector<ClientState> >::iterator iter;
			if (tmp.count(clientSocket) == 0)
				return notOnThatChannel(clientSocket, channel[i]);
			for(iter = tmp.begin(); iter != tmp.end(); iter++)
			{
				if(iter->first != clientSocket)
				{
					std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel[i] + " :" + message + "\r\n";
					send(iter->first, msg.c_str(), msg.length(), 0);
				}
			}
		}
		else
		{
			for (size_t j = 0; j < this->fds.size(); j++)
			{
				if (this->clientStates[this->fds[j].fd].nickname == channel[i])
				{
					std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel[i] + " :" + message + "\r\n";
					send(this->fds[j].fd, msg.c_str(), msg.length(), 0);
					break ;
				}
				else if (j == this->fds.size() - 1)
				{
					noSuchNickChannelReply(clientSocket, channel[i]);
					break ;
				}
			}
		}
	}
}
