/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:00:18 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/28 20:54:02 by bbenidar         ###   ########.fr       */
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
		std::string		topicTimeSetting;
		int		channelUserLimit;
		std::map<int, std::vector<ClientState> >	channelModerators;
		std::map<int, std::vector<ClientState> >	channelClients;
		std::map<int, std::vector<ClientState> >	channelBannedClients;
		std::map<int, std::vector<ClientState> >	channelInvitedClients;
		bool 	channelIsPrivate;
		bool	channelTopicModeratorOnly;
		
	public:
		Channels(){};
		Channels(int usermode,int clientSocket,std::string name, std::string topic, std::string password, int limit, std::vector<ClientState> user);
		~Channels();
		void	setChannelName(const std::string& name);
		void	setChannelPassword(const std::string& password);
		void	setChannelTopic(const std::string& topic);
		void	setChannelUserLimit(int limit);
		void    setChannelprivateMode(bool mode);
		void	setChannelClients(int clientSocket, const std::vector<ClientState>& user);
		void	setChannelModerators(int clientSocket, const std::vector<ClientState>& user);
		void	setChannelBannedClients(int clientSocket, const std::vector<ClientState>& user);
		void	setChannelInvitedClients(int clientSocket, ClientState& user);
		void	setTopicTime(const std::string& time);
		std::string	getTopicTime() const;
		void    removeModerator(int clientSocket);
		std::string	getChannelName() const;
		std::string	getChannelPassword() const;
		std::string	getChannelTopic() const;
		bool getClientisInChannel(int clientSocket) const;
		// std::string	getChannelMode() const;
		std::string	getChannelPrivMode() const;
		bool	getifClientIsBanned(int clientSocket) const;
		bool	getifClientIsInvited(int clientSocket) const;
		bool	getifClientIsModerator(int clientSocket) const;
		bool	getifChannelIsPrivate() const;
		int		getChannelUserLimit() const;
		int getChannelClientsSize() const;
		void printChannelClients() const;
		bool getPassMode() const;
		void KickClient(int clientSocket);
		void setChannelTopicModeratorOnly(bool mode);
		bool getChannelTopicModeratorOnly() const;
		void sendBroadcastMessage(const std::string& message, int clientSocket);
		std::map<int, std::vector<ClientState> >	getChannelClients() const;

};

enum ChannelRole{
	USER,
	ADMIN,
	OWNER,
};

#endif