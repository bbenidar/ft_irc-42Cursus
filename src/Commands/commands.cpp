/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/26 11:05:57 by bbenidar         ###   ########.fr       */
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
				command == "KICK" ? 7 :
				command == "MODE" ? 8 :
				command == "TOPIC" ? 9 : 10;
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
		case 8:
			handleChannelMode(command + " " + parameters, clientSocket);
			break ;
		case 9:
			handleTopicCommand(command + " " + parameters, clientSocket);
			break ;
		default:
			break;
	}
}