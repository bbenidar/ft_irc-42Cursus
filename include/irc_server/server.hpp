/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:57:49 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 15:44:27 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <poll.h>
# include <vector>
# include <iostream>

# define BUFFER_SIZE 4096

class Server
{

private:
	int				serverSocket;
	unsigned short	port;
	std::vector<struct pollfd>	fds;

public:

	void	setupServerSocket( void );
	void	setPort( unsigned short port );
	void	parsePortNumber( const std::string& input );
	void	initializePollStructure();
	void	runServerLoop();
	void	acceptNewClient();
	void	handleClientCommunication(size_t clientIndex);
};

#endif