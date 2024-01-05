/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/06 00:54:13 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::setPort( unsigned short port )
{
	this->port = port;
}

void	Server::parsePortNumberAndPassword( const std::string& s_port, const std::string& password )
{
	unsigned short		port;
	std::stringstream	portNumber(s_port);

	portNumber >> port;
	if (s_port[0] == ' ' || s_port[0] == '-' || !portNumber.eof() || portNumber.fail())
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->port = port;
	if (password.length() == 0)
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
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
		throw std::runtime_error(SOCKET_CREATION);
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
			std::string msg = ":" + this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " PRIVMSG " + channel + " :" + message + "\r\n";
			return (send(this->fds[i].fd, msg.c_str(), msg.length(), 0), true);
		}
	}
	return (sendwrongUserMessage(clientSocket, channel), true);
}

int countChanels(const std::string& channels)
{
	int chanelnum = 0;
	for (size_t i = 0; i < channels.length(); i++)
	{
		if (channels[i] == ',')
			chanelnum++;
	}
	if (channels[channels.length() - 1] != ',')
		chanelnum++;
	return chanelnum;
}

std::string* split(const std::string& channels, char c)
{
	std::string* chanel = new std::string[countChanels(channels)];
	int j = 0;
	for (size_t i = 0; i < channels.length(); i++)
	{
		if (channels[i] == c)
			j++;
		else
			chanel[j] += channels[i];
	}
	return chanel;
}

bool Server::handelJoinchannel(const std::string& msge, int clientSocket)
{
	(void)clientSocket;
	std::string channels = removeMsgCommand(msge);
	if (channels.length() == 0)
		return false;
	std::string passwords = msge.substr(msge.find(channels) + channels.length() + 1);
	if (passwords.length() == 0)
		return false;
	//need to split the chanels in 2d array
	int chanelnum = countChanels(channels);
	std::string* chanel;
	chanel = split(channels, ',');
	for (int i = 0; i < chanelnum; i++)
	{
		if(this->channelClients.count(chanel[i]))
		{
			this->channelClients[chanel[i]].insert(std::pair<int, ClientState>(clientSocket, this->clientStates[clientSocket]));
		}
		else
		{
			std::map<int, ClientState> newmap;
			newmap.insert(std::pair<int, ClientState>(clientSocket, this->clientStates[clientSocket]));
			this->channelClients.insert(std::pair<std::string, std::map<int, ClientState> >(chanel[i], newmap));
		}
	}
	return true;
}

bool Server::handleCommand(int clientSocket, const std::string& message)
{
    if (message.substr(0, 4) == "JOIN")
        return handelJoinchannel(message, clientSocket);
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
		this->clientStates.erase(this->fds[clientIndex].fd);
		close(this->fds[clientIndex].fd);
		if (recvBytes == 0)
			std::cout << BOLD FG_RED "➖ Client disconnected\n" FG_DEFAULT;
		else
			perror("recv");
		this->fds.erase(this->fds.begin() + clientIndex);
	}
	std::string	message(buffer, recvBytes);
	std::cout << message << std::endl;
	if (message[0] == '\n')
		return ;
	std::string	command = getCommand(this->fds[clientIndex].fd, message);
	std::string parameters = getParameters(this->fds[clientIndex].fd, command, message);
	if (parameters.empty())
		return ;
	if (!isClientFullyAuthenticated(this->fds[clientIndex].fd))
		authenticateClient(this->fds[clientIndex].fd, command, parameters);
	else{
		if(!handleCommand(this->fds[clientIndex].fd, message))
					sendwrongCommandMessage(this->fds[clientIndex].fd);
	}
}

void	Server::my_send( int clientSocket, int num
	, const std::string& part1, const std::string& part2 )
{
	std::string	hostname = ":IRCserver";
	std::string	nickname = this->clientStates[clientSocket].nickname;
	std::string	numeric =	(num == 1) ? " 001 " + nickname + part1 + nickname + part2:
							(num == 2) ? " 002 " + nickname + part1 + hostname.substr(1, 10) + part2:
							(num == 3) ? " 003 " + nickname + part1 + part2:
							(num == 4) ? " 004 " + nickname + " " + hostname + part1:
							(num == 5) ? " 005 " + nickname + part1: "";
	std::string	endPart = "\r\n";
	std::string	fullMessage = hostname + numeric + endPart;
	send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
}

void	Server::connectionRegistration( int clientSocket, const std::string& command )
{
	std::string	registrationMsg = ":IRCServer 462 " + command + " :You have not registered\r\n";
	if (command == "NICK" && !this->clientStates[clientSocket].isAuthenticated)
		send(clientSocket, registrationMsg.c_str(), registrationMsg.length(), 0);
	else if (command == "USER" && !this->clientStates[clientSocket].hasNick)
		send(clientSocket, registrationMsg.c_str(), registrationMsg.length(), 0);
	else if (command != "PASS" && !this->clientStates[clientSocket].isAuthenticated
		&& !this->clientStates[clientSocket].hasNick)
		send(clientSocket, registrationMsg.c_str(), registrationMsg.length(), 0);
}

void	Server::sendRegistrationMessages( int clientSocket )
{
	time_t now = time(0);
	char* date_time = ctime(&now);

	my_send(clientSocket, 1, " :Welcome ", " to the IRCServer !");
	my_send(clientSocket, 2, " :Your host is ", " , running version 1.0 !");
	my_send(clientSocket, 3, " :This server was created ", date_time);
	my_send(clientSocket, 4, " version 1.0", "");
	my_send(clientSocket, 5, " are supported by this server", "");
}

void	Server::authenticateClient( int clientSocket, const std::string& command, const std::string& parameters  )
{
	ClientState	state;

	if (command.length() != 0)
		connectionRegistration(clientSocket, command);
	this->clientStates.insert(std::pair<int, ClientState>(clientSocket, state));
	if (!handlePassCommand(clientSocket, command, parameters))
		return ;
	if (!handleNickCommand(clientSocket, command, parameters))
		return ;
	if (!handleUserCommand(clientSocket, command, parameters))
		return ;

	sendRegistrationMessages(clientSocket);
}
