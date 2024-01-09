/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 10:16:17 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/09 10:38:11 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <vector>
# include <iostream>

# define MAX_QUESTIONS	20

class Bot
{
private:

	std::vector<std::vector<std::string> >	questions;

public:
	// Bot();
	// ~Bot();

	void	getQuestions( void);
	void	botCommand( int clientSocket );
	void	fillVector(const std::string& questions, const std::string& answer1, const std::string& answer2, const std::string& answer3, const std::string& correctAnswer);
};

#endif