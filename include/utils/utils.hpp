/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:33:47 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/03 20:47:08 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

void	sendWelcomeMessage(int clientSocket);
void	sendAuthenticationInstructions(int clientSocket);
void sendwrongCommandMessage(int clientSocket);
std::string removeMsgCommand(const std::string& fullMessage);
void sendwrongUserMessage(int clientSocket, std::string& nickname);

#endif