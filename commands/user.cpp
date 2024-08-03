#include "commands.hpp"

void user(std::string message, std::vector<ClientInfo>::iterator &it, std::string hostname)
{
	if (it->authenticated && it->isNicked)
	{
		if (it->isregistered)
		{
			sendToClient(*it, it->socket_fd, ERR_ALREADYREGISTRED());
			return;
		}
		if (count_stream(message) != 5)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("USER"));
			return;
		}
		std::string command;
		std::stringstream ss(message);
		ss >> command;
		ss >> command;
		it->user_name = command;
		ss >> command;
		ss >> command;
		it->server_name = command;
		ss >> command;
		it->real_name = command;
		it->isregistered = true;
		sendToClient(*it, it->socket_fd, RPL_WELCOME(it->nick, it->user_name, hostname));
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}
