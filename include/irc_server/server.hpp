/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:57:49 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/08 17:00:30 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <map>
# include <poll.h>
# include <vector>
# include <iostream>

# define BUFFER_SIZE 4096

struct ClientState {
	bool isAuthenticated;
	bool hasNick;
	bool hasUser;
	std::string nickname;
	std::string username;
	std::string realname;
	std::string hostname;
	std::string servername;

	ClientState() : isAuthenticated(false), hasNick(false), hasUser(false) {}
	
};

class Channels
{
	private:
		std::string		channelName;
		std::string		channelPassword;
		std::string		channelTopic;
		std::string		channelMode;
		int		channelUserLimit;
		std::map<int, std::vector<ClientState> >	channelClients;
		std::map<int, std::vector<ClientState> >	channelModerators;
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
		bool getPassMode() const;
		std::vector<ClientState>	getChannelClients() const;

};


enum ChannelRole{
	USER,
	ADMIN,
	OWNER,
};

class Server
{

	private:
		int				serverSocket;
		std::string		serverPassword;
		unsigned short	port;
		std::vector<struct pollfd>	fds;
		std::map<int, ClientState>	clientStates;	
		std::map<std::string, Channels>	channels;

	public:

		void	runServerLoop( void );
		void	acceptNewClient( void );
		void	setNonBlocking(int fd);
		void	setupServerSocket( void );
		void	signalHandler( int signal );
		void	setPort( unsigned short port );
		void	initializePollStructure( void );
		bool	isClientFullyAuthenticated( int clientSocket );
		void	handleClientCommunication( size_t clientIndex );
		void	parsePortNumberAndPassword( const std::string& s_port, const std::string& serverPassword );

		bool	isNicknameAvailable( const std::string& nickname );
		bool	handlePassCommand( int clientSocket, const std::string& command, const std::string& parameters );
		bool	handleNickCommand( int clientSocket, const std::string& command, const std::string& parameters );
		bool	handleUserCommand( int clientSocket, const std::string& command, const std::string& parameters );
		void	authenticateClient( int clientSocket, const std::string& command, const std::string& parameters );

		void	sendRegistrationMessages( int clientSocket );
		void	my_send( int clientSocket, int num, const std::string& part1, const std::string& part2 );
		void	connectionRegistration( int clientSocket, const std::string& command );
		bool 	handleCommand(int clientSocket, const std::string& message);
		bool 	send_message(const std::string& msge, int clientSocket);
		bool handelJoinchannel(const std::string& msge, int clientSocket);

};




#endif