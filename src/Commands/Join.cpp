/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:25:32 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/26 22:17:08 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <cstring>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
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
	{
		std::string tmp = ":IRCServer 461 " + command +" :Not enough parameters\r\n";
		send(clientSocket, tmp.c_str(), strlen(tmp.c_str()), 0);
		return false;
	}
	return true;

}



void Server::handleJoinchannel(const std::string& msge, int clientSocket, const std::string& command)
{
	if (!checkNumberOfParams(msge, clientSocket, command))
		return ;
	std::string channelsstr = removeMsgCommand(msge);
	if (channelsstr.length() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
	std::string passwords = returnpassonyl(msge);
	std::vector<std::string> chanel;
	std::vector<std::string> pass;
	chanel = split(channelsstr, ',');
	if (chanel.size() != 0)
		pass = split(passwords, ',');
	for (int i = 0; i < (int)chanel.size(); i++)
	{
		if (chanel[i].at(0) == '#')
			chanel[i].erase(0, 1);
		std::cout<<"- " << chanel[i] << std::endl;
		if (!this->channels.count(chanel[i])) {
            std::vector<ClientState> user;
            user.push_back(this->clientStates[clientSocket]);

            if (!pass.empty() && i < static_cast<int>(pass.size())) {
                Channels newChannel(ADMIN, clientSocket, chanel[i], "", pass[i], 1e9, user);
                this->channels.insert(std::pair<std::string, Channels>(chanel[i], newChannel));
            } else {
                Channels newchanel(ADMIN, clientSocket, chanel[i], "", "", 1e9, user);
                this->channels.insert(std::pair<std::string, Channels>(chanel[i], newchanel));
            }
        }
		else
		{
			if (channels[chanel[i]].getClientisInChannel(clientSocket))
			{
				std::string tmp = ":IRCserver 443 " + chanel[i] + " :is already on channel\r\n";
				send(clientSocket, tmp.c_str() , tmp.size(), 0);
				return ;
			}
			if(channels[chanel[i]].getifClientIsBanned(clientSocket))
			{
				std::string tmp = ":IRCServer 474 " + chanel[i] + " :Cannot join channel (+b)\r\n";
				send(clientSocket, tmp.c_str() , tmp.size(), 0);
				return ;
			}
			if (channels[chanel[i]].getChannelClientsSize() >= channels[chanel[i]].getChannelUserLimit())
			{
				std::string tmp = ":IRCServer 471 " + chanel[i] + " :Cannot join channel (+l)\r\n";
				send(clientSocket, tmp.c_str() , tmp.size(), 0);
				return ;
			}
			if (channels[chanel[i]].getifChannelIsPrivate() == true)
			{
				if(channels[chanel[i]].getifClientIsInvited(clientSocket) == false)
				{
					std::string tmp = ":IRCServer 473 " + chanel[i] + " :Cannot join channel (+i)\r\n";
					send(clientSocket, tmp.c_str() , tmp.size(), 0);
					return ;
				}
			}
			if (channels[chanel[i]].getPassMode())
			{
                if ((i < (int)pass.size() && pass[i] != channels[chanel[i]].getChannelPassword()) || pass.empty() || i >= static_cast<int>(pass.size()))
                {
					std::string tmp = ":IRCServer 475 " + chanel[i] + " :Cannot join channel (+k)\r\n";
                    send(clientSocket, tmp.c_str() , tmp.size(), 0);
                    return ;
                }
                else if (i < (int)pass.size() && pass[i] == channels[chanel[i]].getChannelPassword())
                {
                    std::vector<ClientState> user;
                    user.push_back(this->clientStates[clientSocket]);
                    this->channels[chanel[i]].setChannelClients(clientSocket, user);
					if(channels[chanel[i]].getChannelTopic() != "")
					{
						std::string tmp = ":IRCserver 332 " + chanel[i] + " :" + channels[chanel[i]].getChannelTopic() + "\r\n";
						send(clientSocket, tmp.c_str() , tmp.size(), 0);
					}
					else
					{
						std::string tmp = ":IRCserver 332 " + chanel[i] + " :No topic is set\r\n";
						send(clientSocket, tmp.c_str() , tmp.size(), 0);
					}
                }
			}
			else{
				std::vector<ClientState> user;
				user.push_back(this->clientStates[clientSocket]);
				this->channels[chanel[i]].setChannelClients(clientSocket, user);
				if (channels[chanel[i]].getChannelTopic() != "")
				{
					std::string tmp = ":IRCserver 332 " + chanel[i] + " :" + channels[chanel[i]].getChannelTopic() + "\r\n";
					send(clientSocket, tmp.c_str() , tmp.size(), 0);
				}
				else
				{
					std::string tmp = ":IRCserver 332 " + chanel[i] + " :No topic is set\r\n";
					send(clientSocket, tmp.c_str() , tmp.size(), 0);
				}
			}
				
		}
	}
	return ;
}