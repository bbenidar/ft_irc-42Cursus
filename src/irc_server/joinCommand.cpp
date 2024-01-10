/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:25:32 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/10 15:18:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <poll.h>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"



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
	if (tmp.size() == 2)
		return "";
	pass = tmp[2];
	return pass;
}

void Server::handelJoinchannel(const std::string& msge, int clientSocket)
{
	(void)clientSocket;
	std::string channelsstr = removeMsgCommand(msge);
	if (channelsstr.length() == 0)
		return ;

	std::string passwords = returnpassonyl(msge);
	std::cout << "passwords: " << passwords << std::endl;
	std::vector<std::string> chanel;
	std::vector<std::string> pass;
	chanel = split(channelsstr, ',');
	if (chanel.size() != 0)
		pass = split(passwords, ',');
	for (int i = 0; i < (int)chanel.size(); i++)
	{
		std::cout<<"- " << chanel[i] << std::endl;
		if (!this->channels.count(chanel[i])) {
            std::vector<ClientState> user;
            user.push_back(this->clientStates[clientSocket]);

            if (!pass.empty() && i < static_cast<int>(pass.size())) {
                std::cout << "new channel: " << chanel[i] << " with pass " << pass[i] << std::endl;
                Channels newChannel(ADMIN, clientSocket, chanel[i], "", pass[i], "", 100, user);
                this->channels.insert(std::pair<std::string, Channels>(chanel[i], newChannel));
            } else {
                std::cout << "new channel no pass " << std::endl;
                Channels newchanel(ADMIN, clientSocket, chanel[i], "", "", "", 100, user);
                this->channels.insert(std::pair<std::string, Channels>(chanel[i], newchanel));
            }
        }
		else
		{
			if (channels[chanel[i]].getPassMode())
			{
                if (pass.empty() || i >= static_cast<int>(pass.size())) {
                    std::cout << "password need" << std::endl;
                    return;
                }
                else if (pass[i] != channels[chanel[i]].getChannelPassword())
                {
                    std::cout << "password wrong" << std::endl;
                    return;
                }
                else if (pass[i] == channels[chanel[i]].getChannelPassword())
                {
                    std::cout << "password correct" << std::endl;
                    std::vector<ClientState> user;
                    user.push_back(this->clientStates[clientSocket]);
                    this->channels[chanel[i]].setChannelClients(clientSocket, user);
                }
                else
                    std::cout << "password need" << std::endl;
				std::cout << "password need" << std::endl;
			}
			else{
				std::cout << "no password need" << std::endl;
				std::vector<ClientState> user;
				user.push_back(this->clientStates[clientSocket]);
				this->channels[chanel[i]].setChannelClients(clientSocket, user);	
			}
				
		}
	}
	std::map<std::string, Channels>::iterator it;
	for (it = channels.begin(); it != channels.end(); ++it) {
		std::cout << it->first << std::endl;
		it->second.printChannelClients();
	}
	std::cout << "im here" << std::endl;
	return ;
}