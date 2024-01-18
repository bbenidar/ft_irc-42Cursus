/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:00:18 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/18 16:08:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
#define CHANNELS_HPP

# include <map>
# include <vector>
# include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../include/utils/utils.hpp"
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"

struct ClientState;
class Server;

class Channels
{
	private:
		std::string		channelName;
		std::string		channelPassword;
		std::string		channelTopic;
		std::string		channelMode;
		int		channelUserLimit;
		std::map<int, std::vector<ClientState> >	channelModerators;
		std::map<int, std::vector<ClientState> >	channelClients;
		std::map<int, std::vector<ClientState> >	channelBannedClients;
		std::map<int, std::vector<ClientState> >	channelInvitedClients;
		bool 	channelIsPrivate;
		
	public:
		Channels(){};
		Channels(int usermode,int clientSocket,std::string name, std::string topic, std::string password, std::string mode, int limit, std::vector<ClientState> user);
		~Channels();
		void	setChannelName(const std::string& name);
		void	setChannelPassword(const std::string& password);
		void	setChannelTopic(const std::string& topic);
		void	setChannelMode(const std::string& mode0);
		void	setChannelUserLimit(int limit);
		void	setChannelClients(int clientSocket, const std::vector<ClientState>& user);
		void	setChannelModerators(int clientSocket, const std::vector<ClientState>& user);
		std::string	getChannelName() const;
		std::string	getChannelPassword() const;
		std::string	getChannelTopic() const;
		std::string	getChannelMode() const;
		std::string	getChannelPrivMode() const;
		bool	getifClientIsBanned(int clientSocket) const;
		int		getChannelUserLimit() const;
		int getChannelClientsSize() const;
		void printChannelClients() const;
		bool getPassMode() const;
		std::map<int, std::vector<ClientState> >	getChannelClients() const;

};

enum ChannelRole{
	USER,
	ADMIN,
	OWNER,
};

#endif