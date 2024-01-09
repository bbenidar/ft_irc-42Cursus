/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 10:16:33 by moudrib           #+#    #+#             */
/*   Updated: 2024/01/09 10:22:11 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include "../../include/Bot/Bot.hpp"

void	Bot::fillVector(const std::string& question, const std::string& answer1,
	const std::string& answer2, const std::string& answer3, const std::string& correctAnswer)
{
	std::string	qst = question;
	std::string	ans1 = answer1;
	std::string	ans2 = answer2;
	std::string	ans3 = answer3;
	std::string	correct = correctAnswer;
	std::vector<std::string>	element;

	element.reserve(5);
	element.push_back(qst);
	element.push_back(ans1);
	element.push_back(ans2);
	element.push_back(ans3);
	element.push_back(correct);

	this->questions.push_back(element);
}

void	Bot::getQuestions( void)
{
	fillVector("1", "1", "1", "1", "1");
	fillVector("2", "2", "2", "2", "2");
	fillVector("3", "3", "3", "3", "3");
	fillVector("4", "4", "4", "4", "4");
	fillVector("5", "5", "5", "5", "5");
	fillVector("6", "6", "6", "6", "6");
	fillVector("7", "7", "7", "7", "7");
	fillVector("8", "8", "8", "8", "8");
	fillVector("9", "9", "9", "9", "9");
	fillVector("10", "10", "10", "10", "10");
}

void	Bot::botCommand( int clientSocket )
{
	for (int i = 0; i < MAX_QUESTIONS; i++)
	{
		send(clientSocket, this->questions[i][0].c_str(), this->questions[i][0].length(), 0);
		
	}
}