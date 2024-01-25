/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 12:07:16 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/25 20:44:50 by bbenidar         ###   ########.fr       */
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


void Server::handleChannelMode(const std::string& msge, int clientSocket)
{
    int flag = 0;
	std::cout << msge << std::endl;
	std::string chanName = removeMsgCommand(msge);
	if (chanName.size() == 0)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
	std::string Modetype = msge.substr(msge.find(chanName) + chanName.length());
	size_t	begin = Modetype.find_first_not_of(" \n\r", 0);
	size_t	end = Modetype.find_first_of(" \n\r", begin);
	if (begin == std::string::npos)
	{
		std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return ;
	}
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
        flag = 1;
        Modetype.erase(Modetype.begin());
    }
    else if (Modetype.at(0) == '-')
    {
        flag = 2;
        Modetype.erase(Modetype.begin());
    }
    else
    {
        std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (Modetype.size() == 0)
    {
        std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    std::cout << "Modetype |" << Modetype << "| chanName : |" << chanName << "| modeparam : |"<< modeParam << "|" <<std::endl;
    if (Modetype.find_first_not_of("itkol\n\r", 0) != std::string::npos)
    {
        std::string	notEnoughMsg = ":IRCServer 472 " + chanName + " :is unknown mode char to me\r\n";
        send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
        return ;
    }
    if (Modetype.find('i') != std::string::npos)
    {
        std::map<std::string, Channels>::iterator	it2;
        for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
            if (it2->first == chanName)
                break ;
        if (it2 == this->channels.end())
        {
            std::string	notEnoughMsg = ":IRCServer 403 " + chanName + " :No such channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (it2->second.getifClientIsModerator(clientSocket) == false)
        {
            std::string	notEnoughMsg = ":IRCServer 482 " + chanName + " :You're not channel operator\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (flag == 2)
            it2->second.setChannelprivateMode(false);
        else if (flag == 1)
            it2->second.setChannelprivateMode(true);
    }
    if (Modetype.find('o') != std::string::npos)
    {
        if (modeParam == "")
        {
            std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        std::map<std::string, Channels>::iterator	it2;
        for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
            if (it2->first == chanName)
                break ;
        if (it2 == this->channels.end())
        {
            std::string	notEnoughMsg = ":IRCServer 403 " + chanName + " :No such channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (it2->second.getifClientIsModerator(clientSocket) == false)
        {
            std::string	notEnoughMsg = ":IRCServer 482 " + chanName + " :You're not channel operator\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        std::map<int, ClientState>::iterator	it;
        for (it = this->clientStates.begin(); it != this->clientStates.end(); it++)
            if (it->second.nickname == modeParam)
                break ;
        if (it == this->clientStates.end())
        {
            std::string	notEnoughMsg = ":IRCServer 401 " + modeParam + " :No such nick/channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        std::map<int, std::vector<ClientState> > tmp = it2->second.getChannelClients();
        if (tmp.count(it->first) == 0)
        {
            std::string	notEnoughMsg = ":IRCServer 441 " + modeParam + " " + chanName + " :They aren't on that channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (flag == 2)
            it2->second.removeModerator(it->first);
        else if (flag == 1)
        {
            
            std::cout << "soc sender "<< clientSocket << "soc resever" << it->first << std::endl;
            std::vector<ClientState> tmp2 = tmp[it->first];
            it2->second.setChannelModerators(it->first, tmp2);
            it2->second.printChannelClients();
        }
    }
    if (Modetype.find('k') != std::string::npos)
    {
        if (modeParam == "")
        {
            std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        std::map<std::string, Channels>::iterator	it2;
        for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
            if (it2->first == chanName)
                break ;
        if (it2 == this->channels.end())
        {
            std::string	notEnoughMsg = ":IRCServer 403 " + chanName + " :No such channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (it2->second.getifClientIsModerator(clientSocket) == false)
        {
            std::string	notEnoughMsg = ":IRCServer 482 " + chanName + " :You're not channel operator\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (flag == 2)
            it2->second.setChannelPassword("");
        else if (flag == 1)
            it2->second.setChannelPassword(modeParam);
    }
    if (Modetype.find('l') != std::string::npos)
    {
        if (modeParam == "")
        {
            std::string	notEnoughMsg = ":IRCServer 461 PRIVMSG :Not enough parameters\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        std::map<std::string, Channels>::iterator	it2;
        for (it2 = this->channels.begin(); it2 != this->channels.end(); it2++)
            if (it2->first == chanName)
                break ;
        if (it2 == this->channels.end())
        {
            std::string	notEnoughMsg = ":IRCServer 403 " + chanName + " :No such channel\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (it2->second.getifClientIsModerator(clientSocket) == false)
        {
            std::string	notEnoughMsg = ":IRCServer 482 " + chanName + " :You're not channel operator\r\n";
            send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
            return ;
        }
        if (flag == 2)
            it2->second.setChannelUserLimit(0);
        else if (flag == 1)
        {
            std::stringstream geek(modeParam);
            int x = 0;
            geek >> x;
            it2->second.setChannelUserLimit(x);
        }
    }
	
}