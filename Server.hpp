#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <vector>
# include <unistd.h>
# include <arpa/inet.h>
# include <string>
# include <map>
# include <sstream>


# include "Client.hpp"
# include "numeric.hpp"
# include "Channel.hpp"

class ClientInfo;
class Channel;

class	Server
{
	private:
    	std::string passw;
    	int         port;
	public:
		std::map<std::string, Channel>	channels;
		int								_socket;
		int								_bin;
		bool							registered;
		int								_lis;
		struct sockaddr_in 				_sin;
		std::string						hostname;
		Server();
		Server(const std::string &pass, const int &port);
		~Server();
		void		set(std::string passw, int port);
		std::string	getPassw(void) const;
		int			getPort(void) const;
		void		setup(void);
		void		irc_loop(void);
		void		parse(char *buffer, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients);
		void    	process(std::string message, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients);

};

int		count_stream(std::string str);

#endif