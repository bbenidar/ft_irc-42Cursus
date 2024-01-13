/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:03:17 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/12 16:57:40 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

bool	validNickname( const std::string& nickname )
{
	size_t i = -1, length = nickname.length();
	while (++i < length && nickname[i] == ' ');
	if (i == length)
		return false;
	for (i = 0; i < nickname.length(); i++)
		if (!isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '\n')
			return false;
	return true;
}

void	Bot::parseArguments( const std::string& s_port, const std::string& password, const std::string& nickname )
{
	unsigned short		port;
	std::stringstream	portNumber(s_port);

	portNumber >> port;
	if (s_port[0] == ' ' || s_port[0] == '-' || !portNumber.eof() || portNumber.fail() || port < 1024)
		throw std::invalid_argument(INVALID_PORT_NUMBER "\n  " VALID_PORT_NUMBER);
	this->port = port;
	if (password.length() == 0)
		throw std::invalid_argument(INVALID_PASSWORD);
	this->serverPassword = password;
	if (!validNickname(nickname))
		throw std::invalid_argument(INVALID_NICKNAME);
	this->nickname = nickname;
	this->server = "127.0.0.1";
	this->isAuthenticated = false;
}

