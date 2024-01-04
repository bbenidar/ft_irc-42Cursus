/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 20:47:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/utils/utils.hpp"

void	Server::setPort( unsigned short port )
{
	this->port = port;
}

void	Server::parsePortNumberAndPassword( const std::string& s_port, const std::string& password )
{
	unsigned short		port;
	std::stringstream	portNumber(s_port);

	portNumber >> port;
	if (s_port[0] == ' ' || s_port[0] == '-'
		|| !portNumber.eof() || portNumber.fail())
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->port = port;
	if (password.length() == 0)
	{
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	}
	this->serverPassword = password;
}

void Server::setNonBlocking(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror("Error setting non-blocking mode for socket");
	}
}

void	Server::setupServerSocket( void )
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == -1)
	{
		throw std::runtime_error(SOCKET_CREATION);
	}
	// std::cout << BOLD "Socket created\n";

	setNonBlocking(this->serverSocket);

	sockaddr_in	serverAddress;
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->port);
	
	if (bind(this->serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
	{
		close(this->serverSocket);
		throw std::runtime_error(SOCKET_BINDING);
	}
	// std::cout << BOLD "successfully binding the socket\n";

	if (listen(this->serverSocket, 1) == -1)
	{
		close(this->serverSocket);
		throw std::runtime_error(LISTENING_ERROR);
	}
}

void	Server::initializePollStructure()
{
	struct pollfd	newSocket;

	newSocket.fd = this->serverSocket;
	newSocket.events = POLLIN;
	this->fds.push_back(newSocket);
}



bool Server::send_message(const std::string& msge, int clientSocket)
{
	std::string channel = removeMsgCommand(msge);
	if (channel.length() == 0)
		return false;
	std::string message = msge.substr(msge.find(channel) + channel.length() + 1);
	if (message.length() == 0)
		return false;
	for (size_t i = 0; i < this->fds.size(); i++)
	{
		if (this->clientStates[this->fds[i].fd].nickname == channel)
		{
			std::string msg = BOLD FG_LGRAY "➤ " + this->clientStates[clientSocket].nickname + FG_DEFAULT  + message + "\n";
			return (send(this->fds[i].fd, msg.c_str(), msg.length(), 0), true);
		}
	}
	return (sendwrongUserMessage(clientSocket, channel), true);
}

bool Server::handleCommand(int clientSocket, const std::string& message)
{
    if (message.substr(0, 4) == "JOIN")
        return true;
    if (message.substr(0, 7) == "PRIVMSG")
        return send_message(message, clientSocket);
    if (message.substr(0, 4) == "NICK")
        return true;
    return false;
}

void Server::handleClientCommunication(size_t clientIndex)
{
	char	buffer[BUFFER_SIZE];
	int		recvBytes = recv(this->fds[clientIndex].fd, buffer, sizeof(buffer), 0);

	if (recvBytes <= 0)
	{
		if (recvBytes == 0)
		{
			this->clientStates.erase(this->fds[clientIndex].fd);
			std::cout << BOLD FG_RED "➖ Client disconnected\n" FG_DEFAULT;
		}
		else
			perror("recv");
		close(this->fds[clientIndex].fd);
		this->fds.erase(this->fds.begin() + clientIndex);
	}
	std::string	message(buffer, recvBytes);
	if (!isClientFullyAuthenticated(this->fds[clientIndex].fd))
		authenticateClient(this->fds[clientIndex].fd, message);
	else
	{
			if(!handleCommand(this->fds[clientIndex].fd, message))
				sendwrongCommandMessage(this->fds[clientIndex].fd);
	}
}


void	Server::authenticateClient( int clientSocket, const std::string& message )
{
	ClientState	state;

	this->clientStates.insert(std::pair<int, ClientState>(clientSocket, state));
	if (!handlePassCommand(clientSocket, message))
		return ;
	if (!handleNickCommand(clientSocket, message))
		return ;
	if (!handleUserCommand(clientSocket, message))
		return ;
}
