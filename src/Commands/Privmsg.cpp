/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:13:28 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/19 12:35:26 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
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

// bool atTheendofMsg(const std::string& msge, int beg)
// {
	// while(msge[beg])
	// {
		
	// 	if (msge[beg] != ' ' && msge[beg] != '\r' && msge[beg] != '\n')
	// 		return (false);
	// 	beg++;
	// }
	// return (true);
// }


void Server::send_message(const std::string& msge, int clientSocket)
{
	std::cout << msge << std::endl;
	std::string channels = removeMsgCommand(msge);
	if (channels.size() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
	}
	std::string message = msge.substr(msge.find(channels) + channels.length());
	size_t	begin = message.find_first_not_of(" \n\r", 0);
	size_t	end = message.find_last_not_of(" \n\r", message.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
	{
		noMessageToSend(clientSocket);
		return ;
	}
	message =  message.substr(begin, end - begin + 1) + "\r\n";
	std::cout << "message : " << message << std::endl;
	std::vector<std::string> channel;
	channel = split(channels, ',');
	channel = removeDpicates(channel);
	std::cout << "channel size : " << channel.size() << std::endl;
	for (size_t i = 0; i < channel.size(); i++)
	{
		    if (channel[i].at(0) == '#')
			{
				channel[i].erase(0, 1);
				std::cout<< "channel[i] hna: " << channel[i] << std::endl;
				std::map<std::string, Channels>::iterator it;
				for (it = this->channels.begin(); it != this->channels.end(); it++)
				{
					if (it->first == channel[i])
					{
						std::cout << "channel found" << std::endl;
						std::map<int, std::vector<ClientState> > tmp = it->second.getChannelClients();
						std::map<int, std::vector<ClientState> >::iterator iter;
						for(iter = tmp.begin(); iter != tmp.end(); iter++)
						{
							if(iter->first != clientSocket)
							{
								std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel[i] + " :" + message + "\r\n";
								send(iter->first, msg.c_str(), msg.length(), 0);
							}
						}
						break ;
					}
				}
			}
			else
			{
		        std::cout << "chan : " << channel[i] << std::endl;
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
                        sendwrongUserMessage(clientSocket, channel[i]);
                        break ;
                    }
				}
			}
	}
	return ;
}
