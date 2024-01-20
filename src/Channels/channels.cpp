/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:58:24 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/20 13:53:30 by bbenidar         ###   ########.fr       */
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




Channels::Channels(int usermode,int clientSocket,std::string name, std::string topic, std::string password, std::string mode, int limit, std::vector<ClientState> user)
{
    this->channelIsPrivate = false;
    this->channelName = name;
    this->channelTopic = topic;
    this->channelPassword = password;
    this->channelMode = mode;
    this->channelUserLimit = limit;
    this->channelClients[clientSocket] = user;
    if (usermode == ADMIN)
        this->channelModerators[clientSocket] = user;
    if (password.length() != 0)
        this->channelIsPrivate = true;
}

Channels::~Channels()
{
}

void Channels::setChannelName(const std::string& name)
{
    this->channelName = name;
}

void Channels::setChannelTopic(const std::string& topic)
{
    this->channelTopic = topic;
}

void Channels::setChannelPassword(const std::string& password)
{
    this->channelPassword = password;
}

void Channels::setChannelMode(const std::string& mode)
{
    this->channelMode = mode;
}

void Channels::setChannelUserLimit(int limit)
{
    this->channelUserLimit = limit;
}

void Channels::setChannelClients(int clientSocket, const std::vector<ClientState>& user)
{
    this->channelClients[clientSocket] = user;
}

void Channels::setChannelModerators(int clientSocket, const std::vector<ClientState>& user)
{
    this->channelModerators[clientSocket] = user;
}

bool Channels::getPassMode() const
{
    return this->channelIsPrivate;
}

std::string	Channels::getChannelPassword() const
{
    return this->channelPassword;
}

int Channels::getChannelUserLimit() const
{
    return this->channelUserLimit;
}

int Channels::getChannelClientsSize() const
{
    return this->channelClients.size();
}

bool Channels::getifClientIsBanned(int clientSocket) const
{
    if (this->channelBannedClients.count(clientSocket))
        return true;
    return false;
}

std::string Channels::getChannelMode() const
{
    return this->channelMode;
}

void Channels::printChannelClients() const
{
    std::cout << "channel name: " << this->channelName << std::endl;
    std::cout << "channel topic: " << this->channelTopic << std::endl;
    std::cout << "channel password: " << this->channelPassword << std::endl;
    std::cout << "channel mode: " << this->channelMode << std::endl;
    std::cout << "channel user limit: " << this->channelUserLimit << std::endl;
    std::cout << "channel clients: " << std::endl;
    for (std::map<int, std::vector<ClientState> >::const_iterator it = this->channelClients.begin(); it != this->channelClients.end(); ++it)
    {
        std::cout << "client socket: " << it->first << std::endl;
        for (std::vector<ClientState>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            std::cout << "client nickname: " << it2->nickname << std::endl;
            std::cout << "client username: " << it2->username << std::endl;
            std::cout << "client realname: " << it2->realname << std::endl;
            std::cout << "client hostname: " << it2->hostname << std::endl;
            std::cout << "client servername: " << it2->servername << std::endl;
        }
    }
}

std::map<int, std::vector<ClientState> > Channels::getChannelClients() const
{
    return this->channelClients;
}

bool Channels::getifClientIsInvited(int clientSocket) const
{
    if (this->channelInvitedClients.count(clientSocket))
        return true;
    return false;
}

bool Channels::getifClientIsModerator(int clientSocket) const
{
    if (this->channelModerators.count(clientSocket))
        return true;
    return false;
}

std::string Channels::getChannelName() const
{
    return this->channelName;
}

void Channels::setChannelBannedClients(int clientSocket, const std::vector<ClientState>& user)
{
    this->channelBannedClients[clientSocket] = user;
}

void Channels::setChannelInvitedClients(int clientSocket, ClientState& user)
{
    this->channelInvitedClients[clientSocket].push_back(user);
}