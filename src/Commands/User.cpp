#include <sstream>
#include <arpa/inet.h>
#include "../../include/irc_server/server.hpp"

void	Server::userCommand( int clientSocket )
{
	if (isClientFullyAuthenticated(clientSocket))
	{
		std::string mayNotRegisterMsg = ":IRCServer 462 USER :You may not reregister\r\n";
		send(clientSocket, mayNotRegisterMsg.c_str(), mayNotRegisterMsg.length(), 0);
		return ;
	}
}

bool Server::handleUserCommand( int clientSocket, std::string command, const std::string parameters )
{
	if (this->clientStates[clientSocket].hasUser == true)
		return true;
	if (command != "USER")
		return false;
	size_t	i = 0, pos = 3;
	std::string	word;
	std::stringstream	input(parameters);

	while (!input.eof())
	{
		getline(input, word, ' ');
		if (word.length() > 0)
		{
			i++;
            if (i < 4 && word[0] == ':')
                break ;
			if (i == 1)
				this->clientStates[clientSocket].username = word;
			else if (i == 2)
				this->clientStates[clientSocket].hostname = word;
			else if (i == 3)
				this->clientStates[clientSocket].servername = word;
			else if (i == 4)
			{
				pos = parameters.find(word) + 1;
				this->clientStates[clientSocket].realname = (word[0] == ':') ? parameters.substr(pos, parameters.length() - pos) : word;
				break ;
			}
		}
	}
	if (i != 4)
	{
		std::string    notEnoughMsg = ":IRCServer 461 " + command + " :Not enough parameters\r\n";
		send(clientSocket, notEnoughMsg.c_str(), notEnoughMsg.length(), 0);
		return false;
	}
	this->clientStates[clientSocket].hasUser = true;
	return true;
}
