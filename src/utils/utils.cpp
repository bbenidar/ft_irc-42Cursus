/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:32:29 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/04 21:25:04 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"

void	sendWelcomeMessage(int clientSocket)
{
	std::string welcomeMsg = BOLD FG_GREEN "Welcome to the IRC Server!\n" FG_WHITE;
	send(clientSocket, welcomeMsg.c_str(), welcomeMsg.length(), 0);
}

void	sendAuthenticationInstructions(int clientSocket)
{
	std::string authInstructions = FG_LGRAY
		"  Authentication Instructions:\n"
		"  1. PASS <server_password>: Set the server password.\n"
		"  2. NICK <nickname>: Set your nickname.\n"
		"  3. USER <username> <hostname> <servername> :<realname>: Set your user details.\n"
		"  Note: All three steps are required for successful authentication.\n" FG_DEFAULT;

	send(clientSocket, authInstructions.c_str(), authInstructions.length(), 0);
}

bool	validCommands( const std::string& command )
{
	std::string	commands[5] = { "PASS",
								"NICK",
								"USER",
								"JOIN",
								"PRIVMSG"};

	for (int i = 0; i < 5; i++)	
		if (commands[i] == command)
			return true;
	return false;
}

std::string	getCommand( int clientSocket, const std::string& message )
{
	std::string	command;
	std::stringstream	input(message);

	while (command.length() == 0)
		getline(input, command, ' ');
	int lastChar = command.length() - 1;
	command = (command[lastChar] == '\n') ? command.substr(0, lastChar) : command;
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