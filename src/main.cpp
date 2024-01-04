/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:09:14 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 12:23:24 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include "../include/utils/utils.hpp"
#include "../include/utils/colors.hpp"
#include "../include/irc_server/server.hpp"



// Later in your Server class or server setup code...
// signal(SIGINT, signalHandler);

int main( int argc, char **argv )
{
	Server	server;
	try
	{
		if (argc != 3)
			throw std::invalid_argument(INVALID_ARGUMENTS "\n   " VALID_INPUT);
		std::string	portNumber = argv[1];
		std::string	serverPassword = argv[2];
		server.parsePortNumberAndPassword(portNumber, serverPassword);
		server.setupServerSocket();
		// signal(SIGINT, signalHandler);
		server.runServerLoop();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
