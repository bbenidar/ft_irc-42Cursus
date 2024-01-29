/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 17:16:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/29 17:38:05 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/Channels/channels.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::handlePartCommand( int clientSocket, std::string parameter )
{
	std::string			channel,channels,reason;
	std::stringstream	parameters(parameter);
	parameter.erase(0, parameter.find_first_not_of(" "));
	while (channels.length() == 0)
	{
		getline(parameters, channels, ' ');		
		if (parameters.eof())
			break ;
	}
	if (channels.length() == 0)
		return notEnoughParametersReply(clientSocket, "PART");
	while (reason.length() == 0)
	{
		getline(parameters, reason, ' ');
		if (parameters.eof())
			break ;
	}
	std::stringstream	ss(channels);

	while (!ss.eof())
	{
		getline(ss, channel, ',');
		if (channel.length() > 0)
		{
			std::map<std::string, Channels>::iterator toFind = this->channels.find(channel);
			if (toFind != this->channels.end())
			{
				std::map<int, std::vector<ClientState> >	clients = toFind->second.getChannelClients();
				if (clients.find(clientSocket) == clients.end())
					notOnThatChannel(clientSocket, channel);
				else if (clients.find(clientSocket) != clients.end())
				{
					std::string	partMsg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PART " + channel;
					(reason.length() > 0) ? partMsg.append(" :" + reason + "\r\n") : partMsg.append("\r\n");
					this->channels[channel].sendBroadcastMessage(partMsg, clientSocket);
					this->channels[channel].KickClient(clientSocket);
					if (this->channels[channel].getChannelClients().size() == 0)
						this->channels.erase(channel);
					if (this->channels[channel].getifClientIsModerator(clientSocket))
					{
						this->channels[channel].removeModerator(clientSocket);
						std::map<int, std::vector<ClientState> > tmp = this->channels.begin()->second.getChannelClients();
						this->channels[channel].setChannelModerators(tmp.begin()->first, tmp.begin()->second);
					}
				}
			}
			else
				noSuchChannelReply(clientSocket, channel, "PART ");			
		}
	}
}
