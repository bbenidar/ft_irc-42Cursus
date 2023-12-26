/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:09:14 by moudrib           #+#    #+#             */
/*   Updated: 2023/12/26 15:23:54 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/utils/colors.hpp"
#include "../include/irc_server/server.hpp"

int main( int argc, char **argv )
{
	Server	server;
	try
	{
		if (argc != 3)
			throw std::invalid_argument(INVALID_ARGUMENTS "\n   " VALID_INPUT);
		std::string	input = argv[1];
		server.parsePortNumber(input);
		server.setupServerSocket();
		server.runServerLoop();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
