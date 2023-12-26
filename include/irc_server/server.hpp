/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 11:57:49 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 14:20:05 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <poll.h>
# include <vector>
# include <iostream>

class Server
{

private:
	int				fd_count;
	int				serverSocket;
	int				clientSocket;
	unsigned short	port;
	std::vector<struct pollfd>	fds;

public:

	void	setupServerSocket( void );
	void	setPort( unsigned short port );
	void	parsePortNumber( const std::string& input );
	void	setSocketToNonBlocking();
	void	acceptConnection();
	void	handleClient();
};

#endif