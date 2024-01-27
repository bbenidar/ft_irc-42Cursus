/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 12:07:16 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/27 12:48:08 by moudrib          ###   ########.fr       */
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


void Server::handleChannelMode(const std::string& msge, int clientSocket)
{
	int flag = 0;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.size() >0 && chanName.at(0) == '#')
		chanName.erase(0,1);
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
	if (Modetype.at(0) == '+')
	{
		flag = FLAG_ADD;
		Modetype.erase(Modetype.begin());
	}
	else if (Modetype.at(0) == '-')
	{
		flag = FLAG_REMOVE;
		Modetype.erase(Modetype.begin());
	}
	else
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	if (Modetype.size() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	if (Modetype.find_first_not_of("itkol\n\r", 0) != std::string::npos)
	{
		std::string	reply = ":IRCServer 472 " + chanName + " :is unknown mode char to me\r\n";
		send(clientSocket, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (Modetype.find('i') != std::string::npos)
	{
		std::map<std::string, Channels>::iterator	it2;
		for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
			if (it2->first == chanName)
				break ;
		if (it2 == this->channels.end())
			return noSuchChannelReply(clientSocket, chanName);
		if (it2->second.getifClientIsModerator(clientSocket) == false)
			return notChannelOperatorReply(clientSocket, chanName);
		if (flag == FLAG_REMOVE)
			it2->second.setChannelprivateMode(false);
		else if (flag == FLAG_ADD)
			it2->second.setChannelprivateMode(true);
	}
	if (Modetype.find('o') != std::string::npos)
	{
		if (modeParam == "")
			return notEnoughParametersReply(clientSocket, "PRIVMSG");
		std::map<std::string, Channels>::iterator	it2;
		for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
			if (it2->first == chanName)
				break ;
		if (it2 == this->channels.end())
			return noSuchChannelReply(clientSocket, chanName);
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
			it2->second.removeModerator(it->first);
		else if (flag == FLAG_ADD)
		{
			std::vector<ClientState> tmp2 = tmp[it->first];
			it2->second.setChannelModerators(it->first, tmp2);
			it2->second.printChannelClients();
		}
	}
	if (Modetype.find('k') != std::string::npos)
	{
		if (modeParam == "")
			return notEnoughParametersReply(clientSocket, "PRIVMSG");
		std::map<std::string, Channels>::iterator	it2;
		for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
			if (it2->first == chanName)
				break ;
		if (it2 == this->channels.end())
			return noSuchChannelReply(clientSocket, chanName);
		if (it2->second.getifClientIsModerator(clientSocket) == false)
			return notChannelOperatorReply(clientSocket, chanName);
		if (flag == FLAG_REMOVE)
			it2->second.setChannelPassword("");
		else if (flag == FLAG_ADD)
			it2->second.setChannelPassword(modeParam);
	}
	if (Modetype.find('l') != std::string::npos)
	{
		if (modeParam == "")
			return notEnoughParametersReply(clientSocket, "PRIVMSG");
		std::map<std::string, Channels>::iterator	it2;
		for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
			if (it2->first == chanName)
				break ;
		if (it2 == this->channels.end())
			return noSuchChannelReply(clientSocket, chanName);
		if (it2->second.getifClientIsModerator(clientSocket) == false)
			return notChannelOperatorReply(clientSocket, chanName);
		if (flag == FLAG_REMOVE)
			it2->second.setChannelUserLimit(0);
		else if (flag == FLAG_ADD)
		{
			std::stringstream geek(modeParam);
			int x = 0;
			geek >> x;
			it2->second.setChannelUserLimit(x);
		}
	}
	if (Modetype.find('t') != std::string::npos)
	{
			std::map<std::string, Channels>::iterator	it2;
		for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
			if (it2->first == chanName)
				break ;
		if (it2 == this->channels.end())
			return noSuchChannelReply(clientSocket, chanName);
		if (it2->second.getifClientIsModerator(clientSocket) == false)
			return notChannelOperatorReply(clientSocket, chanName);
		if (flag == FLAG_REMOVE)
			it2->second.setChannelTopicModeratorOnly(false);
		else if (flag == FLAG_ADD)
			it2->second.setChannelTopicModeratorOnly(true);
	}
}