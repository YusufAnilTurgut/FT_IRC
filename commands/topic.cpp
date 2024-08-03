#include "commands.hpp"


void topic(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels)
{
	std::string stream;
	std::stringstream ss(message);
	std::string channelName;
	std::string topic;


	ss >> stream;
	ss >> stream;
	channelName = stream;
	if (!it->isregistered && !it->isNicked && !it->authenticated)
	{
		sendToClient(*it, it->socket_fd, ERR_NOTREGISTERED());
		return ;
	}
	if (count_stream(message) < 2)
	{
		sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS("TOPIC"));
		return ;
	}
	if (channels.find(channelName) == channels.end())
	{
		sendToClient(*it, it->socket_fd, ERR_NOSUCHCHANNEL(it->joined_channels.back()));
		return ;
	}
	if (isValidUser(it->nick, channels[channelName].clients) == -1)
	{
		sendToClient(*it, it->socket_fd, ERR_NOTONCHANNEL(channelName));
		return ;
	}
	if (channels[channelName].admins.find(it->nick) == channels[channelName].admins.end())
	{
		sendToClient(*it, it->socket_fd, ERR_CHANOPRIVSNEEDED(channelName));
		return ;
	}
	ss >> stream;

	if (count_stream(message) == 2)
	{
		if (channels[channelName].getChannelTopic().empty())
			sendToClient(*it, it->socket_fd, RPL_NOTOPIC(it->nick, channelName));
		else
			sendToClient(*it, it->socket_fd, RPL_TOPIC(it->nick, channelName, channels[channelName].getChannelTopic()));
		return ;
	}
	topic = stream;
	if (topic[0] == ':' && topic.size() == 1)
	{
		channels[channelName].setChannelTopic("");
		for (std::vector<ClientInfo>::iterator it4 = channels[channelName].clients.begin(); it4 != channels[channelName].clients.end(); it4++)
		{
			sendToClient(*it4, it4->socket_fd, RPL_NOTOPIC(it4->nick, channelName));
		}
		return ;
	}

	if (topic[0] != ':')
	{
		topic.insert(topic.begin(), ':');
	}
	topic  += " ";
	while (ss >> stream)
	{
		topic += stream + " ";
	}
	if (topic.back() == ' ')
		topic.erase(topic.end());
	channels[channelName].setChannelTopic(topic);
	for (std::vector<ClientInfo>::iterator it4 = channels[channelName].clients.begin(); it4 != channels[channelName].clients.end(); it4++)
	{
		sendToClient(*it4, it4->socket_fd, RPL_TOPIC(it4->nick, channelName, getMessage(message)));
	}
}
