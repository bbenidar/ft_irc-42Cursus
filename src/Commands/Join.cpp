/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:25:32 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/31 15:06:03 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"
#include "../../include/Channels/channels.hpp"

std::vector<std::string> split(std::string s, char del)
{
	std::vector<std::string> res;
	std::string tmp;
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == del)
		{
			res.push_back(tmp);
			tmp = "";
		}
		else
			tmp += s[i];
	}
	if (tmp.length() != 0)
		res.push_back(tmp);
	return res;
}

std::string returnpassonyl(const std::string& fullMessage)
{
	std::string pass;
	std::vector<std::string> tmp = split(fullMessage, ' ');
	if (tmp.size() < 1)
		return "";
	pass = tmp[1];
	return pass;
}

bool checkNumberOfParams(const std::string& msge, int clientSocket, const std::string& command)
{
	std::string modifiedMsg = msge;
	if (modifiedMsg.size() > command.size() + 1 && modifiedMsg[command.size() + 1] == '#') {
        modifiedMsg.erase(command.size() + 1, 1);
    }
	std::vector<std::string> check = split(modifiedMsg, ' ');
	if (check.size() < 1)
		return (notEnoughParametersReply(clientSocket, "JOIN"), false);
	return true;

}



void Server::handleJoinchannel(const std::string& msge, int clientSocket, const std::string& command)
{
	if (!checkNumberOfParams(msge, clientSocket, command))
		return ;
	std::string channelsstr = removeMsgCommand(msge);
	if (channelsstr.length() == 0)
		return notEnoughParametersReply(clientSocket, "PRIVMSG");
	std::string passwords = returnpassonyl(msge);
	std::vector<std::string> channel;
	std::vector<std::string> pass;
	channel = split(channelsstr, ',');
	if (channel.size() != 0)
		pass = split(passwords, ',');
	for (int i = 0; i < (int)channel.size(); i++)
	{
		std::cerr << "n : " << this->channels.count(channel[i]) << "\n";
		if (channel[i].at(0) != '#')
			channel[i] = "#" + channel[i];
		if (!this->channels.count(channel[i])) {
            std::vector<ClientState> user;
            user.push_back(this->clientStates[clientSocket]);
            if (!pass.empty() && i < static_cast<int>(pass.size())) {
                Channels newChannel(ADMIN, clientSocket, channel[i], "", pass[i], 100, user);
                this->channels.insert(std::pair<std::string, Channels>(channel[i], newChannel));
				std::string reply = ":" +  this->clientStates[clientSocket].nickname + "!" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
				send(clientSocket, reply.c_str() , reply.size(), 0);
				std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " :No topic is set\r\n" ;
				send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
				std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :@" + this->clientStates[clientSocket].nickname +"\r\n" ;
				send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
				std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
				send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
            } else {
                Channels newchannel(ADMIN, clientSocket, channel[i], "", "", 100, user);
                this->channels.insert(std::pair<std::string, Channels>(channel[i], newchannel));
				std::string reply = ":" +  this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
				send(clientSocket, reply.c_str() , reply.size(), 0);
				std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " :No topic is set\r\n" ;
				send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
				std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :@" + this->clientStates[clientSocket].nickname +"\r\n" ;
				send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
				std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
				send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
            }
        }
		else
		{
			if (channels[channel[i]].getClientisInChannel(clientSocket))
				return alreadyOnChannel(clientSocket, channel[i], "");
			if (channels[channel[i]].getifClientIsBanned(clientSocket))
				return cannotJoinChannel(clientSocket, "474", channel[i], "(+b)");
			if (channels[channel[i]].getChannelClientsSize() >= channels[channel[i]].getChannelUserLimit())
				return cannotJoinChannel(clientSocket, "471", channel[i], "(+l)");
			if (channels[channel[i]].getifChannelIsPrivate() == true)
				if(channels[channel[i]].getifClientIsInvited(clientSocket) == false)
					return cannotJoinChannel(clientSocket, "473", channel[i], "(+i)");
			if (channels[channel[i]].getPassMode())
			{
                if ((i < (int)pass.size() && pass[i] != channels[channel[i]].getChannelPassword()) || pass.empty() || i >= static_cast<int>(pass.size()) )
					return cannotJoinChannel(clientSocket, "475", channel[i], "(+k)");
                else if (i < (int)pass.size() && pass[i] == channels[channel[i]].getChannelPassword())
                {
                    std::vector<ClientState> user;
                    user.push_back(this->clientStates[clientSocket]);
                    this->channels[channel[i]].setChannelClients(clientSocket, user);
					if(channels[channel[i]].getChannelTopic() != "")
					{
						std::string brdcstMsg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
						this->channels[channel[i]].sendBroadcastMessage(brdcstMsg, clientSocket);
						if (channels[channel[i]].getChannelMode() != "")
						{
							std::string modechan = ":IRCServer 324 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " +" + channels[channel[i]].getChannelMode() + "\r\n" ;
							send(clientSocket, modechan.c_str() , modechan.size(), 0);
						}
						std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " " + channels[channel[i]].getChannelTopic() + "\r\n";
						send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
						std::string getChannelClients = channels[channel[i]].getChannelClientInOneString();
						std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :" + getChannelClients +"\r\n" ;
						send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
						std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
						send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
					}
					else
					{
						std::string brdcstMsg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
						this->channels[channel[i]].sendBroadcastMessage(brdcstMsg, clientSocket);
						if (channels[channel[i]].getChannelMode() != "")
						{
							std::string modechan = ":IRCServer 324 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " +" + channels[channel[i]].getChannelMode() + "\r\n" ;
							send(clientSocket, modechan.c_str() , modechan.size(), 0);
						}
						std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " :No topic is set\r\n" ;
						send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
						std::string getChannelClients = channels[channel[i]].getChannelClientInOneString();
						std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :" + getChannelClients +"\r\n" ;
						send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
						std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
						send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
					}
                }
			}
			else
			{
				std::vector<ClientState> user;
				user.push_back(this->clientStates[clientSocket]);
				this->channels[channel[i]].setChannelClients(clientSocket, user);
				if (channels[channel[i]].getChannelTopic() != "")
				{
					std::string brdcstMsg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
					this->channels[channel[i]].sendBroadcastMessage(brdcstMsg, clientSocket);
					if (channels[channel[i]].getChannelMode() != "")
					{
						std::string modechan = ":IRCServer 324 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " +" + channels[channel[i]].getChannelMode() + "\r\n" ;
						send(clientSocket, modechan.c_str() , modechan.size(), 0);
					}
					std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " " + channels[channel[i]].getChannelTopic() + "\r\n";
					send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
					std::string getChannelClients = channels[channel[i]].getChannelClientInOneString();
					std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :" + getChannelClients +"\r\n" ;
					send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
					std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
					send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
				}
				else
				{
					std::string brdcstMsg = ":" + this->clientStates[clientSocket].nickname + "!~" + this->clientStates[clientSocket].username + "@" + this->clientStates[clientSocket].hostname + " JOIN " + channel[i] + "\r\n";
					this->channels[channel[i]].sendBroadcastMessage(brdcstMsg, clientSocket);
					if (channels[channel[i]].getChannelMode() != "")
					{
						std::string modechan = ":IRCServer 324 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " +" + channels[channel[i]].getChannelMode() + "\r\n" ;
						send(clientSocket, modechan.c_str() , modechan.size(), 0);
					}
					std::string topicRep = ":IRCServer 332 " + this->clientStates[clientSocket].nickname + " " + channel[i] + " :No topic is set\r\n" ;
					send(clientSocket, topicRep.c_str() , topicRep.size(), 0);
					std::string getChannelClients = channels[channel[i]].getChannelClientInOneString();
					std::string adminMsg = ":IRCServer 353 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :" + getChannelClients +"\r\n" ;
					send(clientSocket, adminMsg.c_str() , adminMsg.size(), 0);
					std::string endOfList = ":IRCServer 366 " + this->clientStates[clientSocket].nickname + " = " + channel[i] + " :End of /NAMES list.\r\n" ;
					send(clientSocket, endOfList.c_str() , endOfList.size(), 0);
				}
			}
		}
	}
} 