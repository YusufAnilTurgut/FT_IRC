#include "Server.hpp"
#include "./commands/commands.hpp"

Server::Server(): passw(""), port(0) {}
Server::Server(const std::string &pass, const int &port): passw(pass), port(port) {}
Server::~Server() {}

void	Server::set(std::string passw, int port) 
{ 
    this->passw = passw; this->port = port; 
}

std::string	Server::getPassw(void) const 
{
    return(this->passw); 
}

int		Server::getPort(void) const { return(this->port); }

void	Server::setup()
{
	char host_name[1024];
    this->registered = false;
    int herror = gethostname(host_name, 1024);
	if (herror == -1)
    {
        std::cout << "Host Error" << std::endl;
		exit(1);
    }
	this->hostname = host_name;
    _socket = -1;
    _bin = -1;
    _lis = -1;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        std::cerr << "sock error\n";
        exit(1);
    }
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(this->port);
    _sin.sin_addr.s_addr = INADDR_ANY;

    int reuse = 1;

    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        exit(1);
    }
    _bin = bind(_socket, (struct sockaddr *)&_sin, sizeof(_sin));

    if (_bin)
    {
        std::cerr << "bind error\n";
        exit(1);
    }

    _lis = listen(_socket, 5);

    if (_lis)
    {
        std::cerr << "list error\n";
        exit(1);
    }

    std::cout << "Server is listening\n";
}

void	Server::irc_loop()
{
	fd_set rfds, tmp_rfds;
    int max_sd, new_socket;
    struct sockaddr_in client;
    socklen_t addr_len = sizeof(client);
    std::vector<ClientInfo> clients;

    FD_ZERO(&rfds);
    FD_SET(_socket, &rfds);
    max_sd = _socket;

    std::cout << "Server is waiting for connections on port " << this->port << std::endl;

    while (true)
    {
        tmp_rfds = rfds;

        if (select(max_sd + 1, &tmp_rfds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "select error\n";
            break;
        }

        if (FD_ISSET(_socket, &tmp_rfds))
        {

            if ((new_socket = accept(_socket, (struct sockaddr *)&client, &addr_len)) == -1)
            {
                std::cerr << "accept error\n";
                break;
            }
            else
            {
                std::cout << "New connection from " << inet_ntoa(client.sin_addr) << " on socket " << new_socket << std::endl;
                clients.push_back(ClientInfo(new_socket, client));
                FD_SET(new_socket, &rfds);
                if (new_socket > max_sd)
                    max_sd = new_socket;
            }
        }

        for (std::vector<ClientInfo>::iterator it = clients.begin(); it != clients.end();)
        {
            int socketfd = it->socket_fd;
			if (it->hostname.empty())
				it->hostname = this->hostname;
            if (FD_ISSET(socketfd, &tmp_rfds))
            {
                char buffer[1024];
                int bytes_received = recv(socketfd, buffer, sizeof(buffer), 0);

                if (bytes_received <= 0)
                {
                    std::cout << "Client on socket " << it->socket_fd << " disconnected." << std::endl;
                    close(socketfd);
                    FD_CLR(socketfd, &rfds);
                    it = clients.erase(it);
                }
                else
                {
                    buffer[bytes_received] = '\0';
					std::cout << "Received from client " << it->socket_fd << ": " << buffer << std::endl;
                    parse(buffer, it, rfds, clients);
					if (it != clients.end())
                    	++it;
                }
            }
            else
			{
				if (it != clients.end())
					++it;
			}
        }
    }
    close(_socket);
}

void	Server::parse(char *buffer, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients)
{
	std::string message(buffer);
	std::string command;
	std::stringstream ss(message);
	while(std::getline(ss, command))
	{
		process(command, it, rfds, clients);
	}
}

void    Server::process(std::string message, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients)
{
	std::string command;
	std::stringstream ss(message);

	ss >> command;
    if (command == "CAP")
    {	
		ss >> command;
    	cap(it);
    }
    else if (command == "NOTICE")
    {
        notice(message, it, clients, this->channels);
    }
	else if (command == "PING")
	{
		ss >> command; 
		ss >> command;
		ping(command, it);
	}
	else if (command == "NICK")
	{
		nick(message, it, clients);
	}
	else if (command == "USER" )
	{
		user(message, it, this->hostname);
	}
	else if (command == "JOIN")
	{
		join(message, it, this->channels);
	}
	else if (command == "WHO")
	{
		who(message, it, clients, this->channels);
	}
	else if (command == "PART")
	{
		part(message, it, this->channels);
	}
	else if (command == "PRIVMSG")
	{
		privmsg(message, it, clients, this->channels);
	}
	else if (command == "NAMES")
	{
	 	names(message, it, this->channels, clients);
	}
	else if (command == "QUIT")
	{
		quit(message, it, rfds, clients, this->channels);
		return;
	}
	else if (command == "PASS")
	{
		pass(message, it, this->passw);
	}
    else if (command == "MODE")
	{
		mode(message, it, this->channels);
	}
	else if (command == "KICK")
	{
		kick(it, this->channels, message);
	}
    else if (command == "TOPIC")
    {
        topic(message, it, this->channels);
    }
	else
	{
		sendToClient(*it, it->socket_fd, ERR_UNKNOWNCOMMAND(command));
	}
}
