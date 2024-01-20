/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 19:38:30 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/20 14:12:05 by bbenidar         ###   ########.fr       */
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


void Server::handleInvitechannel(const std::string& msge, int clientSocket, const std::string& command)
{
    (void)command;
    (void)clientSocket;
   	std::cout << msge << std::endl;
	std::string clientName = removeMsgCommand(msge);
	if (clientName.size() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
	std::string channel = msge.substr(msge.find(clientName) + clientName.length());
	size_t	begin = channel.find_first_not_of(" \n\r", 0);
	size_t	end = channel.find_last_not_of(" \n\r", channel.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
    if (channel[begin] == '#')
        begin++;
	channel =  channel.substr(begin, end - begin + 1);
	std::cout << "channel " << channel << " clientName : " << clientName << std::endl;

    std::map<int, ClientState>::iterator	it;
    for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
        if (it->second.nickname == clientName)
            break ;
    if (it == this->clientStates.end())
    {
        std::string	notEnoughMsg = ":IRCServer 401 " + clientName + " :No such nick/channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    std::map<std::string, Channels>::iterator	it2;
    for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
        if (it2->first == channel)
            break ;
    if (it2 == this->channels.end())
    {
        std::string	notEnoughMsg = ":IRCServer 403 " + channel + " :No such channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (it2->second.getifClientIsBanned(it->first))
    {
        std::string	notEnoughMsg = ":IRCServer 404 " + channel + " " + clientName + " :Cannot send to channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
    if (tmp.count(clientSocket)== 0)
    {
        std::string	notEnoughMsg = ":IRCServer 442 " + channel + " :You're not on that channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (it2->second.getifClientIsModerator(clientSocket) == false)
    {
        std::string	notEnoughMsg = ":IRCServer 482 " + channel + " :You're not channel operator\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (tmp.count(it->first))
    {
        std::string	notEnoughMsg = ":IRCServer 443 " + channel + " " + clientName + " :is already on channel\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    it2->second.setChannelInvitedClients(it->first, it->second);
    std::string	inviteMsg = ":IRCServer 341 " + channel + " " + clientName + "\r\n";
    send(clientSocket, inviteMsg.c_str(), inviteMsg.length(), 0);
}