/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/07 16:30:35 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/irc_server/server.hpp"

void	Server::processAuthenticatedClientCommand(int clientSocket, const std::string& command, const std::string& parameters )
{
	int cmd	=	command == "PASS" ? 1 :
				command == "NICK" ? 2 : 3;
	switch (cmd)
	{
		case 1:
			passCommand(clientSocket);
			break;
		case 2:
			changeClientNickname(clientSocket, parameters);
			break;
		default:
			break;
	}
}