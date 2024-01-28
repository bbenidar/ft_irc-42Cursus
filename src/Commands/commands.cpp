/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 17:17:46 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/irc_server/server.hpp"

void	Server::processAuthenticatedClientCommand(int clientSocket, const std::string& command, const std::string& parameters )
{
	int cmd	=	command == "PASS"		? 1 :
				command == "NICK"		? 2 :
				command == "USER"		? 3 :
				command == "PART"		? 4 :
				command == "JOIN"		? 5 :
				command == "KICK"		? 6 :
				command == "MODE"		? 7 :
				command == "TOPIC"		? 8 :
				command == "NOTICE"		? 9 :
				command == "INVITE"		? 10 :
				command == "PRIVMSG"	? 11 : 12;
	switch (cmd)
	{
		case 1:
			passCommand(clientSocket);
			break;
		case 2:
			changeClientNickname(clientSocket, parameters);
			break;
		case 3:
			userCommand(clientSocket);
			break;
		case 4:
			handlePartCommand(clientSocket, parameters);
			break;
		case 5:
			handleJoinchannel(parameters, clientSocket, command);
			break;
		case 6:
			handleKickCommand(parameters, clientSocket);
			break ;
		case 7:
			handleChannelMode(parameters, clientSocket);
			break ;
		case 8:
			handleTopicCommand(parameters, clientSocket);
			break ;
		case 9:
			handleNoticeCommand(clientSocket, parameters);
			break;
		case 10:
			handleInvitechannel(parameters, clientSocket);
			break ;
		case 11:
			send_message(parameters, clientSocket);
			break;
		default:
			break;
	}
}