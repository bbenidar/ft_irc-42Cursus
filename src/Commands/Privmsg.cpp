/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:13:28 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/18 16:15:11 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void Server::send_message(const std::string& msge, int clientSocket)
{
	std::cout << msge << std::endl;
	std::string channels = removeMsgCommand(msge);
	if (channels.length() == 0)
		return ;
	std::string message = msge.substr(msge.find(channels) + channels.length() + 1);
	if (message.length() == 0)
	{
		noMessageToSend(clientSocket);
		return ;
	}
	std::vector<std::string> channel;
	channel = split(channels, ',');
	std::cout << "channel size : " << channel.size() << std::endl;
	for (size_t i = 0; i < channel.size(); i++)
	{
		std::cout << "chan : " << channel[i] << std::endl;
		if (channel[i].at(0) == '#')
			{
				channel[i].erase(0, 1);
				std::cout<< "channel[i] hna: " << channel[i] << std::endl;
				std::map<std::string, Channels>::iterator it;
				for (it = this->channels.begin(); it != this->channels.end(); it++)
				{
					// if (it->first == channel[i])
					// {
					// 	std::cout << "channel found" << std::endl;
					// 	std::map<int, std::vector<ClientState> > tmp = it->getChannelClients();
					// 	for (size_t j = 0; j < tmp.size(); j++)
					// 	{
					// 		if (tmp[j].nickname == this->clientStates[clientSocket].nickname)
					// 			continue ;
					// 		std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel[i] + " :" + message + "\r\n";
					// 		send(tmp[j].clientSocket, msg.c_str(), msg.length(), 0);
					// 	}
					// 	return ;
					// }
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
						return ;
					}
					if (this->clientStates[this->fds[j].fd].nickname == channel[i])
					{
						std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel[i] + " :" + message + "\r\n";
						send(this->fds[j].fd, msg.c_str(), msg.length(), 0);
						return ;
					}
				}
			}
		sendwrongUserMessage(clientSocket, channel[i]);
	}
	return ;
}
