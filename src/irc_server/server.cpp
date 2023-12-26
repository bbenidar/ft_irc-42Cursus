/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 14:24:36 by moudrib          ###   ########.fr       */
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

void	Server::setupServerSocket( void )
{
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == -1)
	{
		throw std::runtime_error(SOCKET_CREATION);
	}
	std::cout << BOLD "Socket created\n";

	sockaddr_in	serverAddress;
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(this->port);
	
	if (bind(this->serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
	{
		close(this->serverSocket);
		throw std::runtime_error(SOCKET_BINDING);
	}
	std::cout << BOLD "successfully binding the socket\n";

	if (listen(this->serverSocket, 1) == -1)
	{
		close(this->serverSocket);
		throw std::runtime_error(LISTENING_ERROR);
	}
}
