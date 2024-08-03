#include "commands.hpp"

void cap(std::vector<ClientInfo>::iterator &it)
{
	std::string a = ":" + it->nick + "!" + it->user_name + "@" + it->hostname + ":";
	std::string buffer = a + " "  +  " CAP * LS :multi-refix sasl" + "\r\n";
    send(it->socket_fd, buffer.c_str(), buffer.size(), 0);
}
