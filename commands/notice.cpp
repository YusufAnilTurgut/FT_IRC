#include "commands.hpp"

void notice(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels)
{
	std::string command;
	std::string target;
	std::stringstream ss(message);

	ss >> command;
	ss >> command;
	target = command;
	if (it->authenticated == false && it->isNicked == false && it->isregistered == false)
	{
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
		return ;
	}
	if (count_stream(message) < 3)
	{
		sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("NOTICE"));
		return ;
	}
	if (target[0] == '#')
	{
		if (channels.find(target) == channels.end())
		{
			sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(target));
			return ;
		}
		for (std::vector<ClientInfo>::iterator it2 = channels[target].clients.begin(); it2 != channels[target].clients.end(); it2++)
		{
			if (it->socket_fd != it2->socket_fd)
				sendToClient(*it, it2->socket_fd, getMessage(message));
		}
	}
	else
	{
		if (isValidUser(target, clients) == -1)
		{
			sendToClient(*it, it->socket_fd, ERR_NOSUCHNICK(target));
			return ;
		
		}
		sendToClient(*it, find_nick(clients, target)->socket_fd, getMessage(message));
	}
}
