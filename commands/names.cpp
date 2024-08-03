#include "commands.hpp"

void names(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels, std::vector<ClientInfo> &allClients)
{
	if (it->authenticated && it->isregistered && it->isNicked)
	{
		std::string stream;
		std::stringstream ss(message);
		std::string nicks;

		if (count_stream(message) == 2) //In the channel
		{
			ss >> stream;
			ss >> stream;
			if (channels.find(stream) == channels.end())
			{
				sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(stream));
				return ;
			}
			else if (isValidUser(it->nick, channels[stream].clients) == -1)
			{
				sendToClient(*it, it->socket_fd, ERR_NOTONCHANNEL(stream));
				return;
			}
			std::string channelNameTemp = stream;
			std::string nicks = get_nicks(channels[channelNameTemp].clients, channels[channelNameTemp]);
			for (std::vector<ClientInfo>::iterator it4 = channels[stream].clients.begin(); it4 != channels[stream].clients.end(); it4++)
			{
				sendToClient(*it4, it4->socket_fd, RPL_NAMREPLY(it4->nick, stream, nicks));
				sendToClient(*it4, it4->socket_fd, RPL_ENDOFNAMES(it4->nick, stream));		
			}
		}
		else
		{
			std::string nicks = get_allNicks(allClients);
			sendToClient(*it, it->socket_fd, RPL_NAMREPLY(it->nick, stream, nicks));
			sendToClient(*it, it->socket_fd, RPL_ENDOFNAMES(it->nick, stream));		
		}
	}
}
