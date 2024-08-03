#include "commands.hpp"

void pass(std::string message, std::vector<ClientInfo>::iterator &it, std::string passw)
{
	if (!it->authenticated)
		authenticate(message,it, passw);
	else if (it->authenticated)
		sendToClient(*it, it->socket_fd, ERR_ALREADYREGISTRED());
}
