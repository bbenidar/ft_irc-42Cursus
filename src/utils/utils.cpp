/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:32:29 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 20:46:03 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
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

void sendwrongCommandMessage(int clientSocket)
{
	std::string wrongCommandMsg = FG_RED "   Wrong command\n" FG_DEFAULT;
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

void sendwrongUserMessage(int clientSocket, std::string& nickname)
{
	std::string wrongCommandMsg = FG_RED "   no user with this nickname : " FG_DEFAULT + nickname + "\n";
	send(clientSocket, wrongCommandMsg.c_str(), wrongCommandMsg.length(), 0);
}

std::string removeMsgCommand(const std::string& fullMessage) {
    std::istringstream iss(fullMessage);
    std::string command, channel, message;
    iss >> command >> channel;
    std::getline(iss, message);

    message.erase(0, message.find_first_not_of(" \t\n\r\f\v"));
    message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);

    // if (command == "PRIVMSG" && channel.substr(0, 1) == "#") {
        return channel;
    // }

    // return "";
}