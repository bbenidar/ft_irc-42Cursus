/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:01:54 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/13 13:49:38 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "Bot.hpp"

void	Bot::connectToServer( void )
{
	this->botSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->botSocket == -1)
		throw std::runtime_error(SOCKET_CREATION);

	sockaddr_in	socketAddress;
	std::memset(&socketAddress, 0, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(this->port);

	inet_pton(this->botSocket, this->server.c_str(), &socketAddress.sin_addr); // not allowed

	if (connect(this->botSocket, reinterpret_cast<sockaddr *>(&socketAddress), sizeof(socketAddress)) == -1)
	{
		close(this->botSocket);
		throw std::runtime_error(CONNECTION_FAILED);
	}
}

void	Bot::failedAuthenticationCheck( std::string& message )
{
	if (message.length() < 15)
		return ;
	if (message.substr(0, 15).find("464") != std::string::npos)
	{
		std::cerr << BOLD FG_RED "Error: " FG_DEFAULT "Password incorrect\n";
		exit(EXIT_FAILURE);
	}
	else if (message.substr(0, 15).find("433") != std::string::npos)
	{
		std::cerr << BOLD FG_RED "Error: " FG_DEFAULT "Nickname is already in use\n";
		exit(EXIT_FAILURE);
	}
	this->isAuthenticated = true;
	std::cout << BOLD "Try:   \"PRIVMSG <bot> <someone> <message>\"\n";
}

void	Bot::sendMessage(const std::string& nickname, const std::string& msg)
{
	std::string	message;

	int flag = msg.find('\r') != std::string::npos ? 2 : 1;
	message = "PRIVMSG " + nickname + " " + msg.substr(0, msg.length() - flag) + "\r\n";
	send(this->botSocket, message.c_str(), message.length(), 0);
	std::cerr << "toSend: |" << message << "|\n";
}

void	Bot::handleCommands( const std::string& message )
{
	size_t	pos;
	int flag = message.find('\r') != std::string::npos ? 2 : 1;
	if ((pos = message.find("PRIVMSG")) != std::string::npos)
	{
		if ((pos = message.find(":", pos)) != std::string::npos)
		{
			pos++;
			std::string			nickname;
			std::cerr << "|message: " << message << "|\n";
			std::stringstream	parameters(message.substr(pos, message.length() - pos - flag));
			// std::cerr << "|param: " << parameters.str() << "|\n";
			getline(parameters, nickname, ' ');
			// std::cerr << "|someone: " << nickname << "|\n";
			if (nickname.length() == 0)
				return ;
			pos += nickname.length() + flag - 1;
			if (message.length() > pos )
			{
				std::string	msg = message.substr(pos, message.length() - pos);
				// std::cerr << "|message: " << msg << "|\n";	
				sendMessage(nickname, msg);
			}
			
		}
		// std::cerr << "|nickname: " << nickname << "|\n";
	}
}

void Bot::respondToMessages()
{
	char buffer[BUFFER_SIZE];

	while (true)
	{
		size_t recvBytes = recv(this->botSocket, buffer, sizeof(buffer), 0);
		if (recvBytes <= 0)
			exit(EXIT_FAILURE);
		std::string	message(buffer, recvBytes);
		if (!this->isAuthenticated)
			failedAuthenticationCheck(message);
		handleCommands(message);
		// std::cerr << "|" << message << "|\n";
	}
}

void	Bot::botAuthentication()
{
	std::string	passCommand = "PASS " + this->serverPassword + "\n";
	send(this->botSocket, passCommand.c_str(), passCommand.length(), 0);
	std::string	nickCommand = "NICK " + this->nickname + "\n";
	send(this->botSocket, nickCommand.c_str(), nickCommand.length(), 0);
	std::string	userCommand = "USER IRCBot 0 * :IRCBot\n";
	send(this->botSocket, userCommand.c_str(), userCommand.length(), 0);
}

int main( int argc, char **argv )
{
	Bot	bot;
	try
	{
		if (argc != 4)
			throw std::invalid_argument(INVALID_ARGUMENTS "\n   " VALID_INPUT);
		bot.parseArguments(argv[1], argv[2], argv[3]);
		bot.connectToServer();
		bot.botAuthentication();
		bot.respondToMessages();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
