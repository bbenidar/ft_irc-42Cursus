/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 15:58:24 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/26 11:20:10 by bbenidar         ###   ########.fr       */
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




Channels::Channels(int usermode,int clientSocket,std::string name, std::string topic, std::string password, int limit, std::vector<ClientState> user)
{
	this->channelIsPrivate = false;
	this->channelTopicModeratorOnly = false;
	this->channelName = name;
	this->channelTopic = topic;
	this->channelPassword = password;
	this->channelUserLimit = limit;
	this->channelClients[clientSocket] = user;
	if (usermode == ADMIN)
		this->channelModerators[clientSocket] = user;
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
	if (this->channelPassword.length() != 0)
		return true;
	return false;
}

bool Channels::getClientisInChannel(int clientSocket) const
{
	if (this->channelClients.count(clientSocket))
		return true;
	return false;
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


void Channels::printChannelClients() const
{
	//print chanell moderator
	std::cout << "channel moderator : " << std::endl;
	std::map<int, std::vector<ClientState> >::const_iterator it = this->channelModerators.begin();
	while (it != this->channelModerators.end())
	{
		std::cout << "client socket : " << it->first << std::endl;
		std::cout << "client nickname : " << it->second[0].nickname << std::endl;
		it++;
	}
}

std::map<int, std::vector<ClientState> > Channels::getChannelClients() const
{
	return this->channelClients;
}

bool Channels::getifClientIsInvited(int clientSocket) const
{
	std::map<int, std::vector<ClientState> >::const_iterator it = this->channelInvitedClients.find(clientSocket);
    return it != this->channelInvitedClients.end();
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

void Channels::KickClient(int clientSocket)
{
	this->channelClients.erase(clientSocket);
}
void Channels::setChannelprivateMode(bool mode)
{
	this->channelIsPrivate = mode;
}

bool Channels::getifChannelIsPrivate() const
{
	return this->channelIsPrivate;
}

std::string Channels::getChannelTopic() const
{
	return this->channelTopic;
}

void Channels::removeModerator(int clientSocket)
{
	this->channelModerators.erase(clientSocket);
}

void Channels::setChannelTopicModeratorOnly(bool mode)
{
	this->channelTopicModeratorOnly = mode;
}

bool Channels::getChannelTopicModeratorOnly() const
{
	return this->channelTopicModeratorOnly;
}
