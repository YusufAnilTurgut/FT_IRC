#include "Client.hpp"

 
std::string ClientInfo::getPrefix()
{
    return ":" + this->nick + "!" + this->user_name + "@" + this->hostname + ":";
}

ClientInfo::ClientInfo(int fd, sockaddr_in addr) : socket_fd(fd), address(addr), authenticated(false), attempts(0), isregistered(false), is_connected(true) {}
ClientInfo::~ClientInfo() {}
