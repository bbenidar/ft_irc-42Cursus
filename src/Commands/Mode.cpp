/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 12:07:16 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/21 14:11:58 by bbenidar         ###   ########.fr       */
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
	Modetype =  Modetype.substr(begin, end - begin + 1);
    std::string modeParam = msge.substr(msge.find(Modetype) + Modetype.length());
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
    if (Modetype.find_first_not_of("itkol") == std::string::npos)
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
        it2->second.setChannelprivateMode(true);
    }
	std::cout << "Modetype " << Modetype << " chanName : " << chanName << " modeparam : "<< modeParam << std::endl;
}