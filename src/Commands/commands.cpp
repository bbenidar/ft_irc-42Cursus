/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/20 20:00:56 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/irc_server/server.hpp"

void	Server::processAuthenticatedClientCommand(int clientSocket, const std::string& command, const std::string& parameters )
{
	int cmd	=	command == "PASS" ? 1 :
				command == "NICK" ? 2 :
				command == "JOIN" ? 3 :
				command == "PRIVMSG" ? 4 :
				command == "NOTICE" ? 5 :
				command == "INVITE" ? 6 :
				command == "KICK" ? 7 : 8;
	switch (cmd)
	{
		case 1:
			passCommand(clientSocket);
			break;
		case 2:
			changeClientNickname(clientSocket, parameters);
			break;
		case 3:
			handleJoinchannel(command + " " + parameters, clientSocket, command);
			break;
		case 4:
			send_message(command + " " + parameters, clientSocket);
			break;
		case 5:
			handleNoticeCommand(clientSocket, parameters);
			break;
		case 6:
			handleInvitechannel(command + " " + parameters, clientSocket);
			break ;
		case 7:
			handkleKickCommand(command + " " + parameters, clientSocket);
			break ;
		default:
			break;
	}
}