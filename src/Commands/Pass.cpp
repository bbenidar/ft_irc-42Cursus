/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:03:11 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/20 20:50:02 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void	Server::passCommand( int clientSocket )
{
	if (Server::isClientFullyAuthenticated(clientSocket))
	{
		std::string mayNotRegisterMsg = ":IRCServer 462 PASS :You may not reregister\r\n";
		send(clientSocket, mayNotRegisterMsg.c_str(), mayNotRegisterMsg.length(), 0);
		return ;
	}
}