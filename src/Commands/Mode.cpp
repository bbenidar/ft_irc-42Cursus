/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 12:07:16 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/31 15:17:57 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

const int FLAG_ADD    = 1;
const int FLAG_REMOVE = 2;

// mode grafic !!

void Server::handleChannelMode(const std::string& msge, int clientSocket)
{
	int flag = 0;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.size() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string Modetype = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Modetype.find_first_not_of(" \n\r", 0);
	size_t	end = Modetype.find_first_of(" \n\r", begin);
	if (begin == std::string::npos)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	Modetype =  Modetype.substr(begin, end - begin);
	std::string modeParam = msge.substr(msge.find(Modetype) + Modetype.length());
	begin = modeParam.find_first_not_of(" \n\r", 0);    
	end = modeParam.find_first_of(" \n\r", begin);
	if(end == std::string::npos)
		end = modeParam.length();
	if (begin == std::string::npos)
		modeParam = "";
	else
		modeParam = modeParam.substr(begin, end - begin);
	if (Modetype.size() == 0)
		return notEnoughParametersReply(clientSocket, "MODE");
	std::vector<std::string> modes = splitIntoPairs(Modetype);
	for (int i = 0; i < (int)modes.size(); i++)
	{
		if (modes[i].size() != 2)
			return notEnoughParametersReply(clientSocket, "MODE");
		if (modes[i].find_first_not_of("+-itkol\n\r", 0) != std::string::npos)
		{
			std::string	reply = ":IRCServer 472 " + chanName + " :is unknown mode char to me\r\n";
			send(clientSocket, reply.c_str(), reply.length(), 0);
			return ;
		}
		if (modes[i][0] != '+' && modes[i][0] != '-')
		{
			std::string	reply = ":IRCServer 472 " + chanName + " :is unknown mode char to me\r\n";
			send(clientSocket, reply.c_str(), reply.length(), 0);
			return ;
		}
		if (modes[i].at(0) == '+')
		{
			flag = FLAG_ADD;
			modes[i].erase(modes[i].begin());
		}
		else if (modes[i].at(0) == '-')
		{
			flag = FLAG_REMOVE;
			modes[i].erase(modes[i].begin());
		}
		if (modes[i].find('i') != std::string::npos)
		{
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == chanName)
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, chanName, "MODE " + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, chanName);
			if (flag == FLAG_REMOVE)
			{
				it2->second.setChannelprivateMode(false);
				std::string msg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " MODE " + chanName + " -i" + "\r\n";
				it2->second.sendBroadcastMessage(msg, clientSocket);
				it2->second.setChannelMode("i", false);
			}
			else if (flag == FLAG_ADD)
			{
				it2->second.setChannelprivateMode(true);
				std::string msg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " MODE " + chanName + " +i" + "\r\n";
				it2->second.sendBroadcastMessage(msg, clientSocket);
				it2->second.setChannelMode("i", true);
			}
		}
		else if (modes[i].find('o') != std::string::npos)
		{
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == chanName)
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, chanName, "MODE" + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, chanName);
			std::map<int, ClientState>::iterator	it;
			for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
				if (it->second.nickname == modeParam)
					break ;
			if (it == this->clientStates.end())
				return noSuchNickChannelReply(clientSocket, modeParam);
			std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
			if (tmp.count(it->first) == 0)
				return theyNotOnThatChannel(clientSocket, modeParam, chanName);
			if (flag == FLAG_REMOVE)
			{
				it2->second.removeModerator(it->first);
				std::string msg = ":" + this->clientStates[clientSocket].nickname + " MODE " + chanName + " -o " + modeParam + "\r\n";
				it2->second.sendBroadcastMessage(msg, clientSocket);
			}
			else if (flag == FLAG_ADD)
			{
				std::vector<ClientState> tmp2 = tmp[it->first];
				it2->second.setChannelModerators(it->first, tmp2, this->clientStates[clientSocket].nickname);
				it2->second.printChannelClients();////need remove
			}
		}
		else if (modes[i].find('k') != std::string::npos)
		{
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == chanName)
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, chanName, "MODE" + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, chanName);
			if (flag == FLAG_REMOVE)
			{
				it2->second.setChannelPassword("");
				it2->second.setChannelMode("k", false);
			}
			else if (flag == FLAG_ADD)
			{
				it2->second.setChannelPassword(modeParam);
				it2->second.setChannelMode("k", true);
			}
		}
		else if (modes[i].find('l') != std::string::npos)
		{
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == chanName)
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, chanName, "MODE" + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, chanName);
			if (flag == FLAG_REMOVE)
			{
				it2->second.setChannelUserLimit(100);
				it2->second.setChannelMode("l", false);
			}
			else if (flag == FLAG_ADD)
			{
				std::stringstream geek(modeParam);
				int x = 0;
				geek >> x;
				if (x < 0)
					return notEnoughParametersReply(clientSocket, "MODE"); 
				if (x == 0)
					return notEnoughParametersReply(clientSocket, "MODE");
				if (x > 100)
					x = 100;
				it2->second.setChannelUserLimit(x);
				it2->second.setChannelMode("l", true);
			}
		}
		else if (modes[i].find('t') != std::string::npos)
		{
			std::map<std::string, Channels>::iterator	it2;
			for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
				if (it2->first == chanName)
					break ;
			if (it2 == this->channels.end())
				return noSuchChannelReply(clientSocket, chanName, "MODE" + this->clientStates[clientSocket].nickname + " ");
			if (it2->second.getifClientIsModerator(clientSocket) == false)
				return notChannelOperatorReply(clientSocket, chanName);
			if (flag == FLAG_REMOVE)
			{
				std::string msg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " MODE " + chanName + " -t" + "\r\n";
				it2->second.sendBroadcastMessage(msg, clientSocket);
				it2->second.setChannelTopicModeratorOnly(false);
				it2->second.setChannelMode("t", false);
			}
			else if (flag == FLAG_ADD)
			{
				it2->second.setChannelTopicModeratorOnly(true);
				std::string msg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " MODE " + chanName + " +t" + "\r\n";
				it2->second.sendBroadcastMessage(msg, clientSocket);
				it2->second.setChannelMode("t", true);
			}
		}
	}
}