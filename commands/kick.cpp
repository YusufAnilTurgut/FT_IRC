#include "commands.hpp"
#include "../numeric.hpp"

void    kick(std::vector<ClientInfo>::iterator &sender,std::map<std::string, Channel> &channels, std::string message)
{
	if (sender->authenticated && sender->isregistered && sender->isNicked)
	{
		std::string command;
		std::string channel;
		std::string nickname;
		std::string reason;
		std::stringstream	ss(message);
		std::vector<ClientInfo>::iterator kicked;

		ss >> command;
		ss >> command;
		channel = command;
		ss >> command;
		nickname = command;
		ss >> command;
		reason = command;
		if (isValidUser(sender->nick, channels[channel].clients) == -1)
		{
			sendToClient(*sender,sender->socket_fd, ERR_NOTONCHANNEL(channel));
			return ;
		}
		if (channels[channel].admins.find(sender->nick) != channels[channel].admins.end())
		{
			if (count_stream(message) < 3)
			{
				sendToClient(*sender,sender->socket_fd, ERR_NEEDMOREPARAMS("KICK"));
				return ;
			}
			if (channels.find(channel) == channels.end())
			{
				sendToClient(*sender,sender->socket_fd, ERR_NOSUCHCHANNEL(channel));
				return ;
			}
			if (isValidUser(nickname, channels[channel].clients) == -1)
			{
				sendToClient(*sender,sender->socket_fd, ERR_USERNOTINCHANNEL(nickname, channel));
				return ;
			}
			kicked = find_nick(channels[channel].clients, nickname);
			if (kicked->nick == sender->nick )
			{
				sendToClient(*sender,sender->socket_fd, ERR_CHANOPRIVSNEEDED(channel));
				return ;
			}
			for (std::vector<ClientInfo>::iterator it2 = channels[channel].clients.begin(); it2 != channels[channel].clients.end(); it2++)
				sendToClient(*kicked, it2->socket_fd, message);
			if (channels[channel].admins.find(nickname) != channels[channel].admins.end())
				channels[channel].admins.erase(nickname);
			channels[channel].clients.erase(kicked);
			std::vector<std::string>::iterator joined = find_channel(kicked->joined_channels, channel);
			if (joined != kicked->joined_channels.end())
				kicked->joined_channels.erase(joined);
		}		
		else
		{
			sendToClient(*sender,sender->socket_fd, ERR_CHANOPRIVSNEEDED(channel));
			return ;
		}	
	}	
}
