#include "commands.hpp"

int	checkNick(std::string nick, std::vector<ClientInfo> &clients)
{
	for (std::vector<ClientInfo>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->nick == nick)
			return (0);
	}
	return (1);
}

void nick(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients)
{
	if (it->authenticated)
	{
		std::string stream;
		std::stringstream ss(message);
		ss >> stream;
		ss >> stream;
		
		if(count_stream(message) != 2)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("NICK"));
			return;
		}
		if (stream.empty())
		{
			sendToClient(*it, it->socket_fd, ERR_NONICKNAMEGIVEN());
			return;
		}
		if (it->isNicked == false)
		{
			if (!checkNick(stream, clients))
			{
				sendToClient(*it, it->socket_fd, ERR_NICKNAMEINUSE(stream));
				return;
			}
			it->nick = stream;
			it->isNicked = true;
		}
		else
			sendToClient(*it, it->socket_fd, ERR_ALREADYREGISTRED());
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}
