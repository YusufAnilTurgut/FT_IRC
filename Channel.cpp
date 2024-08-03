#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &name): channelName(name) {}

Channel::~Channel() {}

const std::string &Channel::getChannelName() const
{
	return (this->channelName);
}

void Channel::setChannelName(const std::string &name)
{
	this->channelName = name;
}

const std::string &Channel::getChannelPassw() const
{
	return (this->channelPassw);
}

void Channel::setChannelPassw(const std::string &Passw)
{
	this->channelPassw = Passw;
}

const std::string &Channel::getChannelTopic() const
{
	return (this->channelTopic);
}

void Channel::setChannelTopic(const std::string &Topic)
{
	this->channelTopic = Topic;
}
