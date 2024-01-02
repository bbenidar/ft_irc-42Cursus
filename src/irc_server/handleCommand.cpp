/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 20:35:08 by bbenidar          #+#    #+#             */
/*   Updated: 2024/01/02 20:46:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <sys/socket.h>
#include "../../include/utils/colors.hpp"
#include "../../include/irc_server/server.hpp"


// bool handleCommand(int clientSocket, const std::string& message)
// {
//     if (message.substr(0, 4) == "JOIN")
//         return true;
//     if (message.substr(0, 7) == "PRIVMSG")
//         return true;
//     if (message.substr(0, 4) == "NICK")
//         return true;
//     return false;
// }