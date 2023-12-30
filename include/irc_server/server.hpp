/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:57:49 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/30 16:20:01 by moudrib          ###   ########.fr       */
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

	void	runServerLoop( void );
	void	acceptNewClient( void );
	void	setNonBlocking(int fd);
	void	setupServerSocket( void );
	void	setPort( unsigned short port );
	void	initializePollStructure( void );
	bool	isClientFullyAuthenticated( int clientSocket );
	void	handleClientCommunication( size_t clientIndex );
	void	parsePortNumberAndPassword( const std::string& s_port, const std::string& serverPassword );

	bool	isNicknameAvailable( const std::string& nickname );
	bool	handlePassCommand( int clientSocket, const std::string& input );
	bool	handleNickCommand( int clientSocket, const std::string& input );
	bool	handleUserCommand( int clientSocket, const std::string& input );
	void	authenticateClient( int clientSocket, const std::string& message );
};

#endif