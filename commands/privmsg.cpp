#include "commands.hpp"
#include <sys/socket.h>


void privmsg(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels)
{
	if (it->authenticated && it->isregistered && it->isNicked)
	{
		std::string command;
		std::stringstream ss(message);
		std::vector<ClientInfo>::iterator chan;

		ss >> command;
		ss >> command;
		if (count_stream(message) < 2)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("PRIVMSG"));
			return;
		}
		if (command.front() == '#')
		{
			if (channels.find(command) == channels.end())
			{
				sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(command));
				return;
			}
			std::string channelName = command;
			if (isValidUser(it->nick, channels[channelName].clients) == -1)
			{
				sendToClient(*it, it->socket_fd, ERR_NOTONCHANNEL(channelName));
				return;
			}
			ss >>command;
			chan = channels[channelName].clients.begin();
			while (chan != channels[channelName].clients.end())
			{
				if (chan->nick != it->nick)
				{
					sendToClient(*it, chan->socket_fd, getMessage(message));
				}
				chan++;
			}
			return;
		}
		else
		{
			int fd = isValidUser(command, clients);
			
			if (fd != -1)
			{
				if (it->nick != command)
					sendToClient(*it, fd, getMessage(message));
			}
			else
				sendToClient(*it, it->socket_fd, ERR_NOSUCHNICK(command));
		}
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}
