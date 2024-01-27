/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:25:32 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/27 13:02:03 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

std::vector<std::string> split(std::string s, char del)
{
	std::vector<std::string> res;
	std::string tmp;
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == del)
		{
			res.push_back(tmp);
			tmp = "";
		}
		else
			tmp += s[i];
	}
	if (tmp.length() != 0)
		res.push_back(tmp);
	return res;
}

std::string returnpassonyl(const std::string& fullMessage)
{
	std::string pass;
	std::vector<std::string> tmp = split(fullMessage, ' ');
	if (tmp.size() == 2)
		return "";
	pass = tmp[2];
	return pass;
}

bool checkNumberOfParams(const std::string& msge, int clientSocket, const std::string& command)
{
	std::string modifiedMsg = msge;
	 if (modifiedMsg.size() > command.size() + 1 && modifiedMsg[command.size() + 1] == '#') {
        modifiedMsg.erase(command.size() + 1, 1);
    }
	std::vector<std::string> check = split(modifiedMsg, ' ');
	if (check.size() < 2)
		return (notEnoughParametersReply(clientSocket, "JOIN"), false);
	return true;

}



void Server::handleJoinchannel(const std::string& msge, int clientSocket, const std::string& command)
{
	if (!checkNumberOfParams(msge, clientSocket, command))
		return ;
	std::string channelsstr = removeMsgCommand(msge);
	if (channelsstr.length() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string passwords = returnpassonyl(msge);
	std::vector<std::string> channel;
	std::vector<std::string> pass;
	channel = split(channelsstr, ',');
	if (channel.size() != 0)
		pass = split(passwords, ',');
	for (int i = 0; i < (int)channel.size(); i++)
	{
		if (channel[i].at(0) == '#')
			channel[i].erase(0, 1);
		std::cout<<"- " << channel[i] << std::endl;
		if (!this->channels.count(channel[i])) {
            std::vector<ClientState> user;
            user.push_back(this->clientStates[clientSocket]);

            if (!pass.empty() && i < static_cast<int>(pass.size())) {
                Channels newChannel(ADMIN, clientSocket, channel[i], "", pass[i], 1e9, user);
                this->channels.insert(std::pair<std::string, Channels>(channel[i], newChannel));
            } else {
                Channels newchannel(ADMIN, clientSocket, channel[i], "", "", 1e9, user);
                this->channels.insert(std::pair<std::string, Channels>(channel[i], newchannel));
            }
        }
		else
		{
			if (channels[channel[i]].getClientisInChannel(clientSocket))
				return alreadyOnChannel(clientSocket, channel[i], "");
			if (channels[channel[i]].getifClientIsBanned(clientSocket))
				return cannotJoinChannel(clientSocket, "474", channel[i], "(+b)");
			if (channels[channel[i]].getChannelClientsSize() >= channels[channel[i]].getChannelUserLimit())
				return cannotJoinChannel(clientSocket, "471", channel[i], "(+l)");
			if (channels[channel[i]].getifChannelIsPrivate() == true)
				if(channels[channel[i]].getifClientIsInvited(clientSocket) == false)
					return cannotJoinChannel(clientSocket, "473", channel[i], "(+i)");
			if (channels[channel[i]].getPassMode())
			{
                if ((i < (int)pass.size() && pass[i] != channels[channel[i]].getChannelPassword()) || pass.empty() || i >= static_cast<int>(pass.size()))
					return cannotJoinChannel(clientSocket, "475", channel[i], "(+k)");
                else if (i < (int)pass.size() && pass[i] == channels[channel[i]].getChannelPassword())
                {
                    std::vector<ClientState> user;
                    user.push_back(this->clientStates[clientSocket]);
                    this->channels[channel[i]].setChannelClients(clientSocket, user);
					if(channels[channel[i]].getChannelTopic() != "")
						channelTopic(clientSocket, channel[i], channels[channel[i]].getChannelTopic());
					else
						noTopicIsSet(clientSocket, "332", channel[i]);
                }
			}
			else
			{
				std::vector<ClientState> user;
				user.push_back(this->clientStates[clientSocket]);
				this->channels[channel[i]].setChannelClients(clientSocket, user);
				if (channels[channel[i]].getChannelTopic() != "")
					channelTopic(clientSocket, channel[i], channels[channel[i]].getChannelTopic());
				else
					noTopicIsSet(clientSocket, "332", channel[i]);
			}
		}
	}
}