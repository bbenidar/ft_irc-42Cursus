/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 16:39:51 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::setPort( unsigned short port )
{
	this->port = port;
}

void	Server::parsePortNumber( const std::string& input )
{
	unsigned short		port;
	std::stringstream	portNumber(input);

	portNumber >> port;
	if (input[0] == ' ' || input[0] == '-'
		|| !portNumber.eof() || portNumber.fail())
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->port = port;
}

void Server::setNonBlocking(int fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		perror("Error getting flags for socket");
		return ;
	}
	if (fcntl(fd, flags | O_NONBLOCK, 0) == -1)
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

void	Server::acceptNewClient()
{
	struct pollfd	newSocket;
	sockaddr_in	clientAddress;
	socklen_t	clientAddressSize = sizeof(clientAddress);

	newSocket.fd = accept(this->serverSocket, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressSize);
	if (newSocket.fd == -1)
	{
		perror("accept");
		return ;
	}
	setNonBlocking(newSocket.fd);
	newSocket.events = POLLIN;
	this->fds.push_back(newSocket);
	std::cout << BOLD FG_GREEN << "Client connected\n";
}

void Server::handleClientCommunication(size_t clientIndex)
{
	char	buffer[BUFFER_SIZE];
	int		recvBytes = recv(this->fds[clientIndex].fd, buffer, sizeof(buffer), 0);

	if (recvBytes <= 0)
	{
		if (recvBytes == 0)
			std::cout << BOLD FG_RED << "Client disconnected\n";
		else
			perror("recv");
		close(this->fds[clientIndex].fd);
		this->fds.erase(this->fds.begin() + clientIndex);
	}
}

void	Server::runServerLoop()
{
	initializePollStructure();

	while (true)
	{
		int pollResult = poll(this->fds.data(), this->fds.size(), 0);

		if (pollResult == -1)
		{
			throw std::runtime_error(POLL_FAILURE);
		}
		
		for (size_t clientIndex = 0; clientIndex < this->fds.size(); clientIndex++)
		{
			if (this->fds[clientIndex].revents & POLLIN)
			{
				if (clientIndex == 0)
					acceptNewClient();
				else
					handleClientCommunication(clientIndex);
			}	
		}
	}
}
