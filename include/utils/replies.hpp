/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:51:53 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/28 12:19:14 by bbenidar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

void	notOnThatChannel( int clientSocket, const std::string& channel );
void	notRegisteredReply( int clientSocket, const std::string& command );
void	noSuchChannelReply(int clientSocket, const std::string& channel, std::string command);
void	notChannelOperatorReply( int clientSocket, const std::string& channel );
void	notEnoughParametersReply( int clientSocket, const std::string& command );
void	noSuchNickChannelReply( int clientSocket, const std::string& clientName );
void	nicknameAlreadyInUseReply( int clientSocket, const std::string& nickname );
void	noTopicIsSet( int clientSocket, std::string numeric, const std::string& channel );
void	channelTopic( int clientSocket, const std::string& channel, const std::string& topic );
void	theyNotOnThatChannel( int clientSocket, const std::string& name, const std::string& channel );
void	alreadyOnChannel( int clientSocket, const std::string& channel, const std::string& clientName );
void	cannotJoinChannel( int clientSocket, std::string numeric, std::string channel, std::string mode );

#endif