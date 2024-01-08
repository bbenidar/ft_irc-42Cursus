/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 23:14:07 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/08 16:57:49 by bbenidar         ###   ########.fr       */
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

