/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 17:16:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 20:54:33 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::handlePartCommand( int clientSocket, const std::string& parameter )
{
	std::string			channel,channels;
	std::stringstream	parameters(parameter);
	while (channels.length() == 0)
	{
		getline(parameters, channels, ' ');		
		if (parameters.eof())
			break ;
	}
	if (channels.length() == 0)
		return notEnoughParametersReply(clientSocket, "PART");
	std::cerr << channels << "\n";
	std::stringstream	ss(channels);
	std::map<std::string, std::string>	channelsMap;

	while (!ss.eof())
	{
		getline(ss, channel, ',');
		if (channel.length() > 0)
			channelsMap.insert(std::pair<std::string, std::string>(channel, ""));
	}
	std::map<std::string, std::string>::iterator	it;
	for (it = channelsMap.begin(); it != channelsMap.end(); it++)
	{
		std::cerr << "channel: " << it->first << "\n";
		std::map<std::string, Channels>::iterator toFind = this->channels.find(it->first);
		if (toFind != this->channels.end())
		{
			if (toFind->second.getChannelClients().find(clientSocket) != toFind->second.getChannelClients().end())
			{
				std::cerr << "gone\n";
				this->channels[it->first].KickClient(clientSocket);
				if (this->channels[it->first].getChannelClients().size() == 0)
				{
					this->channels.erase(it->first);
					return ;
				}
				if (this->channels[it->first].getifClientIsModerator(clientSocket))
				{
					this->channels[it->first].removeModerator(clientSocket);
					std::map<int, std::vector<ClientState> > tmp = this->channels.begin()->second.getChannelClients();
					this->channels[it->first].setChannelModerators(tmp.begin()->first, tmp.begin()->second);
				}
			}
		}
	}

}
