/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:09:14 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/09 11:05:30 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "../include/utils/utils.hpp"
#include "../include/utils/colors.hpp"
#include "../include/irc_server/server.hpp"

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
		server.bot.getQuestions();
		server.runServerLoop();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
