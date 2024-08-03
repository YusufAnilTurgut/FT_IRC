#include "commands.hpp"

void ping(std::string command, std::vector<ClientInfo>::iterator &it)
{
	if (command.front() == ':')
		command.erase(command.begin());
	sendToClient(*it, it->socket_fd, " PONG " + command);
}