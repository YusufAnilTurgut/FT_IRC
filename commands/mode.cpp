#include "commands.hpp"

void mode(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels)
{

	if (it->authenticated && it->isregistered && it->isNicked)
	{
		std::string command;
		std::stringstream ss(message);
		std::string name;
		std::string mode;
		std::string	nick;

		ss >> command;
		ss >> command;
		name = command;
		if (count_stream(message) < 2)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("MODE"));
			return;
		}
		if (count_stream(message) == 4)
		{
			ss >> command;
			mode = command;
			ss >> command;
			nick = command;
			if (isValidUser(it->nick, channels[name].clients) == -1)
			{
				sendToClient(*it, it->socket_fd, ERR_NOTONCHANNEL(it->nick));
				return ;
			}
			if (channels[name].admins.find(it->nick) == channels[name].admins.end())
			{
				sendToClient(*it, it->socket_fd, ERR_CHANOPRIVSNEEDED(name));
				return;
			}
			if (isValidUser(nick, channels[name].clients) == -1)
			{
				sendToClient(*it, it->socket_fd, ERR_NOTONCHANNEL(nick));
				return;
			}
			if (mode == "+o")
			{
				channels[name].admins.insert(make_pair(nick, std::string("@" + nick)));
			}
			else if (mode == "-o")
			{
				if (nick == it->nick)
				{
					sendToClient(*it, it->socket_fd, ERR_CHANOPRIVSNEEDED(name));
					return ;
				}
				if (channels[name].admins.find(nick) != channels[name].admins.end())
					channels[name].admins.erase(nick);
				else
				{
					sendToClient(*it, it->socket_fd, ERR_NOSUCHNICK(nick));
					return;
				}
			}
			std::string nicks = get_nicks(channels[name].clients, channels[name]);
			for (std::vector<ClientInfo>::iterator it4 = channels[name].clients.begin(); it4 != channels[name].clients.end(); it4++)
			{
				sendToClient(*it4, it4->socket_fd, RPL_NAMREPLY(it4->nick, name, nicks));
				sendToClient(*it4, it4->socket_fd, RPL_ENDOFNAMES(it4->nick, name));		
			}
		}
		else if (count_stream(message) == 2)
		{
            sendToClient(*it, it->socket_fd, RPL_CHANNELMODEIS(it->nick, "+nt"));
        }
	}
	else
	{
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
	}
}
