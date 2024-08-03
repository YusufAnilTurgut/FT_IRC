#include "commands.hpp"

std::string getMessage(std::string oldMessage)
{
	std::string command;
	std::stringstream ss(oldMessage);

	ss >> command;
	std::string message;
	message += command + " ";
	ss >> command;
	message += command + " ";
	int index = 0;
	
	while(ss >> command)
	{
		if (command.front() != ':' && index == 0)
		{
			message += " :";
			index = 1;
		}
		message += command + " ";
		index = 1;
	}
	if (message.back() == ' ')
		message.erase(message.end());
	return message;
}


int isValidUser(std::string name, std::vector<ClientInfo> &clients)
{
	std::vector<ClientInfo>::iterator it;
	it = clients.begin();
	while(it != clients.end())
	{
		if(it->nick == name)
			return (it->socket_fd);
		it++;
	}
	return (-1);
}

std::string get_nicks(std::vector<ClientInfo> &clients, Channel &channel)
{
	std::string nicks = "";

	for (std::vector<ClientInfo>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (channel.admins.find(it->nick) != channel.admins.end())
			nicks += channel.admins[it->nick] + " ";
		else
			nicks += it->nick + " ";
	}
	return nicks;
}


std::string get_allNicks(std::vector<ClientInfo> &clients)
{
	std::string nicks = "";

	for (std::vector<ClientInfo>::iterator it = clients.begin(); it != clients.end(); it++)
	{
			nicks += it->nick + " ";
	}
	return nicks;
}

std::vector<std::string>::iterator	find_channel(std::vector<std::string> &joined, std::string channel)
{
	std::vector<std::string>::iterator it = joined.begin();
	if (joined.size() == 0)
		return joined.end();
	while (it != joined.end())
	{
		if (*it == channel)
			return it;
		it++;
	}
	return it;
}

std::vector<ClientInfo>::iterator  find_nick(std::vector<ClientInfo>  &clients, std::string nick)
{
    std::vector<ClientInfo>::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->nick == nick)
            return it;
        it++;
    }
    return it;
}
