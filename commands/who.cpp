#include "commands.hpp"
#include "../numeric.hpp"



void who(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels)
{
	if (it->authenticated && it->isregistered && it->isNicked)
	{
		std::string stream;
		std::stringstream ss(message);

		ss >> stream;
		ss >> stream;
		if (count_stream(message) != 2)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("WHO"));
			return;
		}
		if (stream.front() == '#')
		{
			if (channels.find(stream) == channels.end())
			{
				sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(stream));
				return;
			}
			for (std::vector<ClientInfo>::iterator it2 = channels[stream].clients.begin(); it2 != channels[stream].clients.end(); ++it2)
			{
					sendToClient(*it, it->socket_fd, RPL_WHOREPLY(it2->nick, stream, it2->user_name, it2->hostname, "ircserv", it2->nick ,"*", "0", it2->real_name));
			}
			sendToClient(*it, it->socket_fd, RPL_ENDOFWHO(it->nick, stream));
		}
		else
		{
			if (isValidUser(stream, clients)  == -1)
			{
				sendToClient(*it, it->socket_fd, ERR_NOSUCHNICK(it->nick));
				return;
			}
			std::vector<ClientInfo>::iterator nick_it = find_nick(clients, stream);
			sendToClient(*it, it->socket_fd, RPL_WHOREPLY(nick_it->nick, stream, nick_it->user_name, nick_it->hostname, "ircserv", nick_it->nick ,"*", "0", nick_it->real_name));
			sendToClient(*it, it->socket_fd, RPL_ENDOFWHO(it->nick, stream));
		}
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}
