/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:33:47 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/04 21:26:16 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

void	sendWelcomeMessage(int clientSocket);
bool	validCommands( const std::string& command );
void	sendAuthenticationInstructions(int clientSocket);

#endif