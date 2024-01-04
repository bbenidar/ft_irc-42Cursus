/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplexing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 21:23:25 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 12:38:22 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

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
	// sendWelcomeMessage(newSocket.fd);
	// sendAuthenticationInstructions(newSocket.fd);
	setNonBlocking(newSocket.fd);
	newSocket.events = POLLIN;
	this->fds.push_back(newSocket);
	std::cout << BOLD FG_GREEN "➕ Client connected\n" FG_DEFAULT;
}

void	Server::runServerLoop()
{
	initializePollStructure();

	while (true)
	{
		int pollResult = poll(this->fds.data(), this->fds.size(), 0);

		if (pollResult == -1)
			throw std::runtime_error(POLL_FAILURE);
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
