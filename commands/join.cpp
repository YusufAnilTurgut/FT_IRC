#include "commands.hpp"

bool isBadChannelName(std::string name)
{
	int i = 0;
	while (name[i])
	{
		if (name[i] == ',')
			return true;
		i++;
	}
	if (name.front() != '#')
		return true;
	return false;
}


std::vector<ClientInfo>::iterator findId(int id, std::vector<ClientInfo> &clients)
{
	std::vector<ClientInfo>::iterator it = clients.begin();
	while(it != clients.end())
	{
		if(it->socket_fd == id)
			return (it);
		it++;
	}
	return (it);
}

int checkPassw(std::string message, std::map<std::string, Channel> &channels)
{

	std::string stream;
	std::stringstream ss(message);

	ss >> stream;
	ss >> stream;
	std::string channelName = stream;
	ss >> stream; 
	std::string channelPassw = stream;
	if (channels[channelName].getChannelPassw().empty())
		return 1;
	if (channels[channelName].getChannelPassw() == channelPassw)
		return 1;
	return 0;
}


void join(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels)
{
	if (it->authenticated && it->isregistered && it->isNicked)
	{
		std::string stream;
		std::stringstream ss(message);
		std::string nicks;
		std::string admin;
		std::string channelName;
		std::string topic;

		if (count_stream(message) == 1 || count_stream(message) > 3) 
		{
			sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("JOIN"));
			return ;
		}
		ss >> stream;
		ss >> stream;
		if (isBadChannelName(stream) == true)
		{
			sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(stream));
			return ;
		}
		if (channels.find(stream) == channels.end())
		{
			Channel newChannel(stream);
			channels.insert(std::make_pair<std::string, Channel>(stream, newChannel));
			channels[stream].clients.push_back(*it);
			it->joined_channels.push_back(stream);
			it->admin_channels.insert(std::make_pair<std::string, Channel>(stream, newChannel));
			channels[stream].admins.insert(std::make_pair<std::string, std::string>(it->nick, std::string("@" + it->nick)));

			sendToClient(*it, it->socket_fd, "JOIN You are now in channel " + channels[stream].getChannelName());

			sendToClient(*it, it->socket_fd, RPL_NOTOPIC(it->nick, stream));
			sendToClient(*it, it->socket_fd, RPL_NAMREPLY(it->nick, stream, channels[stream].admins[it->nick]));
			sendToClient(*it, it->socket_fd, RPL_ENDOFNAMES(it->nick, stream));
			if (count_stream(message) == 3)
			{
				std::string temp = stream;
				ss >> stream;
				channels[temp].setChannelPassw(stream);
			}
		}
		else
		{
			if (isValidUser(it->nick, channels[stream].clients) != -1)
			{
				sendToClient(*it, it->socket_fd, ERR_USERONCHANNEL(it->nick, stream));
				return ;
			}
			std::vector<ClientInfo>::iterator findID = findId(it->socket_fd, channels[stream].clients);
			if (findID != channels[stream].clients.end())
			{
				findID->nick = it ->nick;
			}
			if (count_stream(message) == 3 && channels[stream].getChannelPassw().empty())
			{
				sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("JOIN"));
				return ;
			}
			if (checkPassw(message, channels) == 0)
			{
				std::string channelName = stream;
				ss >> stream;
				sendToClient(*it, it->socket_fd, ERR_BADCHANNELKEY(it->nick, channelName, stream));
				return ;
			}

			channels[stream].clients.push_back(*it);
			it->joined_channels.push_back(stream);		
	
			channelName = stream;
			sendToClient(*it, it->socket_fd, " JOIN You are now in channel " + channels[channelName].getChannelName());
			if (channels[channelName].getChannelTopic().empty() == true)
				sendToClient(*it, it->socket_fd, RPL_NOTOPIC(it->nick, channelName));
			else
				sendToClient(*it, it->socket_fd, RPL_TOPIC(it->nick, channelName, channels[channelName].getChannelTopic()));
			nicks = get_nicks(channels[channelName].clients, channels[channelName]);
			 for (std::vector<ClientInfo>::iterator it4 = channels[channelName].clients.begin(); it4 != channels[channelName].clients.end(); it4++)
			{
			 	sendToClient(*it4, it4->socket_fd, RPL_NAMREPLY(it4->nick, channelName, nicks));
			 	sendToClient(*it4, it4->socket_fd, RPL_ENDOFNAMES(it4->nick, channelName));		
			}
		}
	}
	else
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
}