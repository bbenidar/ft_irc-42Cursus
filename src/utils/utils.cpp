/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:32:29 by moudrib           #+#    #+#             */
/*   Updated: 2024/02/06 11:46:05 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"

bool setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		return false;
	return true;
}

bool	validCommands( const std::string& command )
{
	std::string	commands[13] = {	"PASS",
									"NICK",
									"USER",
									"PART",
									"JOIN",
									"PONG",
									"KICK",
									"MODE",
									"QUIT",
									"TOPIC",
									"INVITE",
									"NOTICE",
									"PRIVMSG"};

	for (int i = 0; i < 13; i++)	
		if (commands[i] == command)
			return true;
	return false;
}

std::string	getCommand( int clientSocket, const std::string& message )
{
	std::string	command;
	std::stringstream	input(message);

	if (message[0] != ' ')
		getline(input, command, ' ');
	for (size_t i = 0; i < command.length(); i++)
		command[i] = std::toupper(command[i]);
	if (!validCommands(command) || message[0] == ' ')
	{
		std::string	unkonwn = ":IRCServer 421 " + command + " :Unknown command\r\n";
		send(clientSocket, unkonwn.c_str(), unkonwn.length(), 0);
		return "";
	}
	return command;
}

std::string	getParameters( int clientSocket, const std::string& command, const std::string& message )
{
	if (command.empty())
		return "";
	int start = command.length() + 1;
	int flag = (message.find('\r', 0) != std::string::npos) ? 1 : 0;
	std::string	parameters;
	if (message[start] && static_cast<int>(message.length()) > start + flag)
		parameters = message.substr(start, message.length() - start - flag);
	if (((message[start - 1] != ' ' || parameters.length() == 0) && command != "NOTICE"))
		return (notEnoughParametersReply(clientSocket, command), "");
	return parameters;
}

void noMessageToSend(int clientSocket)
{
	std::string wrongCommandMsg = ":IRCServer 412 :No text to send\r\n";
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

std::string removeMsgCommand(const std::string& fullMessage) 
{
	std::string			channel;
	std::stringstream	parameters(fullMessage);
	while (channel.length() == 0)
	{
		getline(parameters, channel, ' ');		
		if (parameters.eof())
			break ;
	}
	return channel;
}

std::string Joinchannelpars(const std::string& msge)
{
	std::string channel = removeMsgCommand(msge);
	if (channel.length() == 0)
		return "";
	std::string message = msge.substr(msge.find(channel) + channel.length() + 1);
	if (message.length() == 0)
		return "";
	return channel;
}

bool	validNickname( int clientSocket, const std::string& nickname )
{
	size_t i = -1, length = nickname.length();
	while (++i < length && nickname[i] == ' ');
	if (i == length)
	{
		std::string	noNicknameMsg = ":IRCServer 431 NICK :No nickname given\r\n";
		send(clientSocket, noNicknameMsg.c_str(), noNicknameMsg.length(), 0);
		return false;
	}
	int	flag = isdigit(nickname[0]) ? 1 : 0;
	for (i = 0; i < nickname.length(); i++)
	{
		if ((!isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '\n') || flag)
		{
			std::string	noNicknameMsg = ":IRCServer 432 NICK :Erroneus nickname\r\n";
			send(clientSocket, noNicknameMsg.c_str(), noNicknameMsg.length(), 0);
			return false;
		}
	}
	return true;
}

std::vector<std::string> splitIntoPairs(const std::string& input) 
{
    std::vector<std::string> result;
    std::string pair;

    for (size_t i = 0; i < input.size(); i += 2) {
        pair = input.substr(i, 2);
        result.push_back(pair);
    }

    return result;
}

void printChanneljoin(std::map<int, ClientState>& clientStates, std::string Topic , int clientSocket, const std::string& channel ,std::string chanelClient, std::string mode)
{
	std::string reply = ":" +  clientStates[clientSocket].nickname + "!" + clientStates[clientSocket].username + "@" + clientStates[clientSocket].hostname + " JOIN " + channel+ "\r\n";
	send(clientSocket, reply.c_str() , reply.size(), 0);
	if (mode != "")
	{
		std::string modechan = ":IRCServer 324 " + clientStates[clientSocket].nickname + " " + channel + " +" + mode + "\r\n" ;
		send(clientSocket, modechan.c_str() , modechan.size(), 0);
	}
	std::string topicRep = ":IRCServer 332 " + clientStates[clientSocket].nickname + " " + channel + " " + Topic +"\r\n" ;
	send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
	std::string adminMsg = ":IRCServer 353 " + clientStates[clientSocket].nickname + " = " + channel + " :" + chanelClient +"\r\n" ;
	send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
	std::string endOfList = ":IRCServer 366 " + clientStates[clientSocket].nickname + " = " + channel + " :End of /NAMES list.\r\n" ;
	send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
}
