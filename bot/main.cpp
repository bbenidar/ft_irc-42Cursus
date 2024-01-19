/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:01:54 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/18 21:49:24 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include "Bot.hpp"

int main( int argc, char **argv )
{
	Bot	bot;
	try
	{
		if (argc != 4)
			throw std::invalid_argument(INVALID_ARGUMENTS "\n   " VALID_INPUT);
		bot.parseArguments(argv[1], argv[2], argv[3]);
		bot.connectToServer();
		bot.botAuthentication();
		bot.respondToMessages();

	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
