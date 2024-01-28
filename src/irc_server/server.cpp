/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 18:05:46 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::parsePortNumberAndPassword( const std::string& s_port, const std::string& password )
{
	unsigned short		port;
	std::stringstream	portNumber(s_port);

	portNumber >> port;
	if (s_port[0] == ' ' || s_port[0] == '-' || !portNumber.eof() || portNumber.fail() || port < 1024)
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->port = port;
	if (password.length() == 0)
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->serverPassword = password;
}

bool Server::isClientFullyAuthenticated(int clientSocket)
{
	return clientStates[clientSocket].isAuthenticated &&
			clientStates[clientSocket].hasNick &&
			clientStates[clientSocket].hasUser;
}

void	Server::sendRegistrationMessages( int clientSocket )
{
	time_t now = time(0);
	char* date_time = ctime(&now);
	std::string	date(date_time, strlen(date_time) - 1);

	my_send(clientSocket, 1, " :Welcome ", " to the IRCServer !");
	my_send(clientSocket, 2, " :Your host is ", ", running version 1.0 !");
	my_send(clientSocket, 3, " :This server was created ", date);
	my_send(clientSocket, 4, " version 1.0", "");
	my_send(clientSocket, 5, " are supported by this server", "");
}

void	Server::setupServerSocket( void )
{
	int option_value = 1;
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == -1)
		throw std::runtime_error(SOCKET_CREATION);
	// std::cout << BOLD "Socket created\n";
	if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
		throw std::runtime_error(SETSOCKOPT);
	if (!setNonBlocking(this->serverSocket))
		return ;
	sockaddr_in	serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->port);
	
	if (bind(this->serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
		throw std::runtime_error(SOCKET_BINDING);
	// std::cout << BOLD "successfully binding the socket\n";
	if (listen(this->serverSocket, 10) == -1)
		throw std::runtime_error(LISTENING_ERROR);
}

void	Server::my_send( int clientSocket, int num
	, const std::string& part1, const std::string& part2 )
{
	std::string	hostname = ":IRCServer ";
	std::string	nickname = this->clientStates[clientSocket].nickname;
	std::string	numeric =	(num == 1) ? "001 " + nickname + part1 + nickname + part2 :
							(num == 2) ? "002 " + nickname + part1 + "IRCServer" + part2 :
							(num == 3) ? "003 " + nickname + part1 + part2 :
							(num == 4) ? "004 " + nickname + " " + hostname + part1 :
							(num == 5) ? "005 " + nickname + part1 : "";
	std::string	endPart = "\r\n";
	std::string	fullMessage = hostname + numeric + endPart;
	send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
}

bool Server::handleClientCommunication(size_t clientIndex)
{
	size_t 	end = std::string::npos;
	char	buffer[BUFFER_SIZE];
	int		recvBytes = recv(this->fd, buffer, sizeof(buffer), 0);
	std::vector <std::string> shouldErase;

	if (recvBytes <= 0)
	{
		for (std::map<std::string, Channels>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		{
			std::map<int, std::vector<ClientState> > tmp = it->second.getChannelClients();
			if (tmp.count(this->fd) != 0)
			{
				it->second.KickClient(this->fd);
				if (this->channels[it->first].getChannelClients().size() == 0)
				{
					shouldErase.push_back(it->first);
					continue ;
				}
				if (this->channels[it->first].getifClientIsModerator(this->fd))
				{
					if (this->channels[it->first].getChannelClients().size() != 0)
					{
						std::map<int, std::vector<ClientState> > tmp = this->channels.begin()->second.getChannelClients();
						this->channels[it->first].setChannelModerators(tmp.begin()->first, tmp.begin()->second);
					}
				}
			}
		}
		for (size_t i = 0 ; i < shouldErase.size(); i++)
			this->channels.erase(shouldErase[i]);
		this->clientStates.erase(this->fd);
		this->clientBuffers.erase(this->fd);
		close(this->fd);
		this->fds.erase(this->fds.begin() + clientIndex);
		std::cout << BOLD FG_RED "➖ Client disconnected\n" FG_DEFAULT;
		return false;
	}
	this->clientBuffers[this->fd].append(buffer, recvBytes);
	std::cerr << "|message: " << this->clientBuffers[this->fd] << "|\n";
	if ((end = this->clientBuffers[this->fd].find('\n')) != std::string::npos)
	{
		std::string			line;
		std::stringstream	input(this->clientBuffers[this->fd]);
		while (!input.eof())
		{
			getline(input, line, '\n');
			if (line.empty())
				break ;
			line = line.find('\r') != std::string::npos ? line.substr(0, line.length() - 1) : line;
			std::string	command = getCommand(this->fd, line);
			std::string parameters = getParameters(this->fd, command, line);
			if (parameters.empty())
				return true;
			if (!isClientFullyAuthenticated(this->fd))
				authenticateClient(this->fd, command, parameters);
			else
				processAuthenticatedClientCommand(this->fd, command, parameters);
		}
		return true;
	}
	return false;
}

void	Server::authenticateClient( int clientSocket, std::string& command, const std::string& parameters  )
{
	ClientState	state;
	bool		isAuthenticated = this->clientStates[clientSocket].isAuthenticated;

	std::string	registrationMsg = ":IRCServer 462 " + command + " :You have not registered\r\n";
	if ((command == "NICK" && !isAuthenticated) || (command == "USER" && !isAuthenticated)
		|| (command != "PASS" && command != "NICK" && command != "USER"
		&& !isClientFullyAuthenticated(clientSocket)))
	{
		send(clientSocket, registrationMsg.c_str(), registrationMsg.length(), 0);
		return ;
	}
	this->clientStates.insert(std::pair<int, ClientState>(clientSocket, state));
	if (!this->clientStates[clientSocket].isAuthenticated)
		handlePassCommand(clientSocket, command, parameters);
	if (!this->clientStates[clientSocket].hasNick)
		handleNickCommand(clientSocket, command, parameters);
	if (!this->clientStates[clientSocket].hasUser)
		handleUserCommand(clientSocket, command, parameters);
	if (isClientFullyAuthenticated(clientSocket))
		sendRegistrationMessages(clientSocket);
}
