/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:28:52 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/26 19:23:56 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/irc_server/server.hpp"

void	Server::processAuthenticatedClientCommand(int clientSocket, const std::string& command, const std::string& parameters )
{
	int cmd	=	command == "PASS"		? 1 :
				command == "NICK"		? 2 :
				command == "USER"		? 3 :
				command == "JOIN"		? 4 :
				command == "KICK"		? 5 :
				command == "MODE"		? 6 :
				command == "TOPIC"		? 7 :
				command == "NOTICE"		? 8 :
				command == "INVITE"		? 9 :
				command == "PRIVMSG"	? 10 : 11;
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
			handleJoinchannel(command + " " + parameters, clientSocket, command);
			break;
		case 5:
			handleKickCommand(command + " " + parameters, clientSocket);
			break ;
		case 6:
			handleChannelMode(command + " " + parameters, clientSocket);
			break ;
		case 7:
			handleTopicCommand(command + " " + parameters, clientSocket);
			break ;
		case 8:
			handleNoticeCommand(clientSocket, parameters);
			break;
		case 9:
			handleInvitechannel(command + " " + parameters, clientSocket);
			break ;
		case 10:
			send_message(command + " " + parameters, clientSocket);
			break;
		default:
			break;
	}
}