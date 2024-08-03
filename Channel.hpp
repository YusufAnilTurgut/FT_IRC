#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

class ClientInfo;                                                       

class Channel
{
	private:
		std::string							channelName;
		std::string							channelPassw;
		std::string							channelTopic;
	public:
		Channel();
		Channel(const std::string &name);
		~Channel();
		std::vector<ClientInfo>					clients;
		std::map<std::string, std::string>		admins;
		const std::string					&getChannelName(void) const;
		void								setChannelName(const std::string &name);
		const std::string					&getChannelPassw(void) const;
		void								setChannelPassw(const std::string &Passw);
		const std::string					&getChannelTopic(void) const;
		void								setChannelTopic(const std::string &Topic);

};

#endif
