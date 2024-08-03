#include "commands.hpp"

std::string	part_string(std::vector<std::string> &tokens)
{
	std::string channel;
	if (tokens.size() == 1)
		return ("PART " + tokens[0]);
	channel = "PART ";
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
	{
		if (tokens.end() - 1 != it)
			channel += *it + ",";
		else
			channel += *it;
	}
	return (channel);
}

void quit(std::string message, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels)
{
	// check if the user is on any channels
	if (it->joined_channels.size() > 0)
		part(part_string(it->joined_channels), it, channels);
	it->is_connected = false;
	sendToClient(*it, it->socket_fd, message);
	close(it->socket_fd);
    FD_CLR(it->socket_fd, &rfds);
	it = clients.erase(it);
}
