/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:59:22 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/19 13:20:51 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
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

