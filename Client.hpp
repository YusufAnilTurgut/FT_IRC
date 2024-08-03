#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <netinet/in.h>
#include "Server.hpp"
# include <sys/socket.h>
# include <iostream>
# include <vector>
# include <unistd.h>
# include <arpa/inet.h>
# include <string>
# include <sstream>

class Server;
class Channel;

class ClientInfo {
    public:
        std::map<std::string, Channel> admin_channels;
		std::vector<std::string>	joined_channels;
        std::string					user_name;
		std::string 				server_name;
        std::string 				nick;
		std::string 				real_name;
		std::string 				hostname;
        int 						socket_fd;
        struct sockaddr_in			address;
		bool						isNicked;
        bool    					authenticated;
        int     					attempts;
		bool 						isregistered;
        bool 						is_connected;
		std::string					getPrefix();
        ClientInfo(int fd, sockaddr_in addr);
		~ClientInfo();
};

void    sendToClient(ClientInfo &sender, int receiverSocket, std::string message);
void    authenticate(std::string message, std::vector<ClientInfo>::iterator &it, std::string passw);



#endif