#include <sstream>
#include <arpa/inet.h>
#include "../../include/utils/replies.hpp"
#include "../../include/irc_server/server.hpp"

void	Server::userCommand( int clientSocket )
{
	if (isClientFullyAuthenticated(clientSocket))
		notRegisteredReply(clientSocket, "USER");
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
				continue;
				// this->clientStates[clientSocket].hostname = this->clientStates[clientSocket].hostname;
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
		return (notEnoughParametersReply(clientSocket, "USER"), false);
	this->clientStates[clientSocket].hasUser = true;
	return true;
}
