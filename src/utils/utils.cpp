/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:32:29 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/18 15:25:28 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"

void setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("Error setting non-blocking mode for socket");
	}
}

bool	validCommands( const std::string& command )
{
	std::string	commands[7] = { "PASS",
								"NICK",
								"USER",
								"JOIN",
								"PRIVMSG",
								"PONG",
								"QUIT"};

	for (int i = 0; i < 7; i++)	
		if (commands[i] == command)
			return true;
	return false;
}

std::string	getCommand( int clientSocket, const std::string& message )
{
	std::string	command;
	std::stringstream	input(message);

	getline(input, command, ' ');
	int lastChar = command.length() - 1;
	command = (command[lastChar] == '\n') ? command.substr(0, lastChar) : command;
	for (size_t i = 0; i < command.length(); i++)
		command[i] = std::toupper(command[i]);
	if (!validCommands(command) || message[0] == ' ')
	{
		std::string	unkonwn = ":IRCServer 421 " + command + " :\r\n";
		send(clientSocket, unkonwn.c_str(), unkonwn.length(), 0);
		return "";
	}
	return command;
}

std::string	getParameters( int clientSocket, const std::string& command, const std::string& message )
{
	int start = command.length() + 1;
	int flag = (message.find('\r', 0) != std::string::npos) ? 2 : 1;
	std::string	parameters = message.substr(start, message.length() - start - flag);
	if ((message[start - 1] != ' ' || parameters.length() == 0))
	{
		std::string	notEnoughMsg = ":IRCServer 461 " + command + " :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return "";
	}
	return parameters;
}
// dyawli matemse7hoch fl merge
void sendwrongCommandMessage(int clientSocket)
{
	std::string wrongCommandMsg = FG_RED "   Wrong command\n" FG_DEFAULT;
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

void sendwrongUserMessage(int clientSocket, std::string& nickname)
{
	std::string wrongCommandMsg = ":IRCServer 401 " + nickname + " :No such nick/channel\r\n";
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

void noMessageToSend(int clientSocket)
{
	std::string wrongCommandMsg = ":IRCServer 412 :No text to send\r\n";
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

std::string removeMsgCommand(const std::string& fullMessage) {
    std::istringstream iss(fullMessage);
    std::string command, channel, message;
    iss >> command >> channel;
    std::getline(iss, message);

    message.erase(0, message.find_first_not_of(" \t\n\r\f\v"));
    message.erase(message.find_first_not_of(" \t\n\r\f\v") + 1);
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
	for (i = 0; i < nickname.length(); i++)
	{
		if (!isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '\n')
		{
			std::string	noNicknameMsg = ":IRCServer 431 NICK :Erroneus nickname\r\n";
			send(clientSocket, noNicknameMsg.c_str(), noNicknameMsg.length(), 0);
			return false;
		}
	}
	return true;
}
