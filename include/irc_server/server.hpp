/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:57:49 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 17:55:48 by bbenidar         ###   ########.fr       */
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



class Server
{

private:
	int				serverSocket;
	std::string		serverPassword;
	unsigned short	port;
	std::vector<struct pollfd>	fds;
	std::map<int, ClientState>	clientStates;	

public:

	void	setupServerSocket( void );
	void	setPort( unsigned short port );
	void	parsePortNumberAndPassword( const std::string& s_port, const std::string& serverPassword );
	void	initializePollStructure();
	void	runServerLoop();
	void	acceptNewClient();
	void	handleClientCommunication(size_t clientIndex);
	void	setNonBlocking(int fd);
	bool	isClientFullyAuthenticated(int clientSocket);

	void	authenticateClient( int clientSocket, const std::string& message );
	bool	handlePassCommand( int clientSocket, const std::string& input );
	bool	handleNickCommand( int clientSocket, const std::string& input );
	bool	handleUserCommand( int clientSocket, const std::string& input );
	bool 	handleCommand(int clientSocket, const std::string& message);
	bool 	send_message(const std::string& msge, int clientSocket);

};

#endif