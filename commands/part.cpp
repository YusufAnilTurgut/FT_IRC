#include "commands.hpp"
#include <sstream>
#include <iostream>

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void	part(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels)
{
	if (it->authenticated && it->isregistered && it->isNicked)
	{	
		std::vector<std::string>::iterator it3;
		std::string stream;
		std::stringstream ss(message);
		int i = 0;
		std::string reason = " ";

		if (count_stream(message) < 2)
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("PART"));
			return ;
		}
		ss >> stream;
		ss >> stream;
		std::vector<std::string> tokens = split(stream, ',');
		while (ss >> stream)
			reason += stream + " ";
		for (std::vector<std::string>::iterator itTokens = tokens.begin(); itTokens != tokens.end(); itTokens++)
		{
			if (channels.find(*itTokens) == channels.end())
			{
				sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(*itTokens));
				continue ;
			}
			
			for (std::vector<ClientInfo>::iterator chanClient = channels[*itTokens].clients.begin(); chanClient != channels[*itTokens].clients.end(); chanClient++)
			{
				if (chanClient->nick == it->nick)
				{
					//if client is admin delete client from admin list
	
					if (channels[*itTokens].admins.find(it->nick) != channels[*itTokens].admins.end())
						channels[*itTokens].admins.erase(it->nick);

					// delete channel name from joined_channels vector in client
					it3 = find_channel(it->joined_channels, *itTokens);
					if (it3 != it->joined_channels.end())
					{
						it->joined_channels.erase(it3);
					}

					if (channels[*itTokens].clients.size() == 1)
					{
						channels.erase(*itTokens);
						sendToClient(*it, it->socket_fd, "PART " + *itTokens);
						i = 1;
						break ;
					}
					else if (channels[*itTokens].clients.size() > 1 && channels[*itTokens].admins.size() == 0)
					{
						std::vector<ClientInfo>::iterator clientit = find_nick(channels[*itTokens].clients, it->nick);
						if (clientit + 1 != channels[*itTokens].clients.end())
							clientit++;
						else
							clientit = channels[*itTokens].clients.begin();
						channels[*itTokens].admins.insert(make_pair(clientit->nick, std::string("@" + clientit->nick)));
					}
					/*
						Exit the channel and send it to all clients in the channel
					*/

					channels[*itTokens].clients.erase(chanClient);
					sendToClient(*it, it->socket_fd, "PART " + *itTokens + reason); 
					for (std::vector<ClientInfo>::iterator it4 = channels[*itTokens].clients.begin(); it4 != channels[*itTokens].clients.end(); it4++)
					{
						sendToClient(*it, it4->socket_fd, "PART " + *itTokens + " :" + it->nick + reason);
					}
					i = 1;
					std::string nicks = get_nicks(channels[*itTokens].clients, channels[*itTokens]);
					for (std::vector<ClientInfo>::iterator it4 = channels[*itTokens].clients.begin(); it4 != channels[*itTokens].clients.end(); it4++)
					{
						sendToClient(*it4, it4->socket_fd, RPL_NAMREPLY(it4->nick, *itTokens, nicks));
						sendToClient(*it4, it4->socket_fd, RPL_ENDOFNAMES(it4->nick, *itTokens));		
					}
					break ;
				}
			}
		}
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}
