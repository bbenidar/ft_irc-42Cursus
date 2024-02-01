/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:32:29 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/29 16:46:57 by moudrib          ###   ########.fr       */
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
	if (message[start])
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
