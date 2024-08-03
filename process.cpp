#include "Server.hpp"
#include "./commands/commands.hpp"

void    sendToClient(ClientInfo &sender, int receiverSocket, std::string message)
{
    std::string buffer = sender.getPrefix() + " "  +  message + "\r\n";
    std::cout << "Sent to client:" << " " << buffer << std::endl;
    send(receiverSocket, buffer.c_str(), buffer.size(), 0);
}

void    authenticate(std::string message, std::vector<ClientInfo>::iterator &it, std::string passw)
{
	std::string command;
	std::stringstream ss(message);

	if (count_stream(message) != 2)
	{
		sendToClient(*it, it->socket_fd, ERR_NEEDMOREPARAMS());
		return;
	}
	ss >> command;
	ss >> command;
    if (command.front() == ':')
        command.erase(command.begin());
    if (command.compare(passw) == 0)
    {
        it->authenticated = true;
    }
    else
    {
        sendToClient(*it, it->socket_fd, ERR_PASSWDMISMATCH());
        it->attempts++;
    }
}
