/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:50:49 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 17:36:23 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <arpa/inet.h>

void	notEnoughParametersReply( int clientSocket, const std::string& command )
{
	std::string	notEnoughMsg = ":IRCServer 461 " + command + " :Not enough parameters\r\n";
	send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
}

void	notChannelOperatorReply( int clientSocket, const std::string& channel )
{
	std::string	notOperatorMsg = ":IRCServer 482 " + channel + " :You're not channel operator\r\n";
	send(clientSocket, notOperatorMsg.c_str(), notOperatorMsg.length(), 0);
}

void	noSuchChannelReply( int clientSocket, const std::string& channel, std::string command)
{
	std::string	noSuchChannelMsg = ":IRCServer 403 " + command + channel + " :No such channel\r\n";
	send(clientSocket, noSuchChannelMsg.c_str(), noSuchChannelMsg.length(), 0);
}

void	nicknameAlreadyInUseReply( int clientSocket, const std::string& nickname )
{
	std::string	alreadyinuseMsg = ":IRCServer 433 " + nickname + " :Nickname is already in use\r\n";
	send(clientSocket, alreadyinuseMsg.c_str(), alreadyinuseMsg.length(), 0);
}

void	noSuchNickChannelReply( int clientSocket, const std::string& clientName )
{
	std::string	noSuchNickChannelMsg = ":IRCServer 401 " + clientName + " :No such nick/channel\r\n";
	send(clientSocket, noSuchNickChannelMsg.c_str(), noSuchNickChannelMsg.length(), 0);
}

void	notRegisteredReply( int clientSocket, const std::string& command )
{
	std::string mayNotRegisterMsg = ":IRCServer 462 " + command + " :You may not reregister\r\n";
	send(clientSocket, mayNotRegisterMsg.c_str(), mayNotRegisterMsg.length(), 0);
}

void	cannotJoinChannel( int clientSocket, std::string numeric, std::string channel, std::string mode )
{
	std::string tmp = ":IRCServer " + numeric + " " + channel + " :Cannot join channel " + mode + "\r\n";
	send(clientSocket, tmp.c_str() , tmp.size(), 0);
}

void	noTopicIsSet( int clientSocket, std::string numeric, const std::string& channel )
{
	std::string noTopicMsg = ":IRCserver " + numeric + " " + channel + " :No topic is set\r\n";
	send(clientSocket, noTopicMsg.c_str() , noTopicMsg.size(), 0);
}

void	alreadyOnChannel( int clientSocket, const std::string& channel, const std::string& clientName )
{
	std::string	reply = ":IRCServer 443 " + channel + " " + clientName + ":is already on channel\r\n";
	send(clientSocket, reply.c_str(), reply.length(), 0);
}

void	notOnThatChannel( int clientSocket, const std::string& channel )
{
	std::string	reply = ":IRCServer 442 " + channel + " :You're not on that channel\r\n";
	send(clientSocket, reply.c_str(), reply.length(), 0);
}

void	theyNotOnThatChannel( int clientSocket, const std::string& name, const std::string& channel )
{
	std::string	reply = ":IRCServer 441 " + name + " " + channel + " :They aren't on that channel\r\n";
	send(clientSocket, reply.c_str(), reply.length(), 0);
}

void	channelTopic( int clientSocket, const std::string& channel, const std::string& topic )
{
	std::string reply =":IRCServer 332 " + channel + ": " + topic + "\r\n";

	send(clientSocket, reply.c_str() , reply.size(), 0);
}
