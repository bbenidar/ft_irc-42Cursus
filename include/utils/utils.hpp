/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 10:33:47 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/02 21:00:00 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

void	sendWelcomeMessage(int clientSocket);
void	sendAuthenticationInstructions(int clientSocket);
void sendwrongCommandMessage(int clientSocket);

#endif