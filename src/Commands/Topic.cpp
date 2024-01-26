/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 10:54:18 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/26 21:22:22 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

void Server::handleTopicCommand(const std::string& msge, int clientSocket)
{
    (void)clientSocket;
	std::string chanName = removeMsgCommand(msge);
    
	if (chanName.size() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
    std::map<std::string, Channels>::iterator	it2;
    for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
        if (it2->first == chanName)
            break ;
    if (it2 == this->channels.end())
    {
        std::string	notEnoughMsg = ":IRCServer 403 " + chanName + " :No such channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
	std::string Topic = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Topic.find_first_not_of(" \n\r", 0);
	size_t	end = Topic.find_last_not_of(" \n\r", Topic.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
	{
        if (it2->second.getChannelTopic() == "")
        {
            std::string	notEnoughMsg = ":IRCServer 331 " + chanName + " :No topic is set\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        else 
        {
            std::string	topicmsg = ":IRCServer 332 " + chanName + " :" + it2->second.getChannelTopic() + "\r\n";
            send(clientSocket, topicmsg.c_str(), topicmsg.length(), 0);
            return ;
        }
	}
	Topic =  Topic.substr(begin, end - begin + 1);
    std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
    if (tmp.count(clientSocket)== 0)
    {
        std::string	notEnoughMsg = ":IRCServer 442 " + chanName + " :You're not on that channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (it2->second.getChannelTopicModeratorOnly() == true && it2->second.getifClientIsModerator(clientSocket) == false)
    {
        std::string	notEnoughMsg = ":IRCServer 482 " + chanName + " :You're not channel operator\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    it2->second.setChannelTopic(Topic);
}