/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:48:36 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/21 12:07:00 by bbenidar         ###   ########.fr       */
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

void Server::handkleKickCommand(const std::string& msge, int clientSocket)
{
	(void)clientSocket;
	std::cout << msge << std::endl;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.size() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
	std::string Nickname = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Nickname.find_first_not_of(" \n\r", 0);
	size_t	end = Nickname.find_last_not_of(" \n\r", Nickname.length() - 1);
	if (begin == std::string::npos || end == std::string::npos)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
	if (Nickname[begin] == '#')
		begin++;
	Nickname =  Nickname.substr(begin, end - begin + 1);
	std::cout << "NIckname " << Nickname << " chanName : " << chanName << std::endl;
	std::vector<std::string> Nams = split(Nickname, ',');
	std::vector<std::string> Chans = split(chanName, ',');
	for (int i = 0; i < (int)Chans.size(); i++)
	{
		for (int j = 0; j < (int)Nams.size(); j++)
		{
			std::map<int, ClientState>::iterator	it;
			for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
				if (it->second.nickname == Nams[j])
					break ;
			if (it == this->clientStates.end())
			{
				std::string	notEnoughMsg = ":IRCServer 401 " + Nams[j] + " :No such nick/channel\r\n";
				send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
				return ;
			}
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == Chans[i])
					break ;
			if (it2 == this->channels.end())
			{
				std::string	notEnoughMsg = ":IRCServer 403 " + Chans[i] + " :No such channel\r\n";
				send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
				return ;
			}
			if (it2->second.getifClientIsModerator(clientSocket) == false)
			{
				std::string	notEnoughMsg = ":IRCServer 482 " + Chans[i] + " :You're not channel operator\r\n";
				send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
				return ;
			}
			std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
			if (tmp.count(it->first) == 0)
			{
				std::string	notEnoughMsg = ":IRCServer 441 " + Nams[j] + " " + Chans[i] + " :They aren't on that channel\r\n";
				send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
				return ;
			}
			this->channels[Chans[i]].KickClient(it->first);
		}
	}

}