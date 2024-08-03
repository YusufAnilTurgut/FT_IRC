#ifndef COMMANDS_HPP
# define COMMANDS_HPP



# include "../Client.hpp"
# include "../Server.hpp"
# include "../Channel.hpp"
# include "../numeric.hpp"

class ClientInfo;
class Channel; 

void    							kick(std::vector<ClientInfo>::iterator &sender,std::map<std::string, Channel> &channels, std::string message);
std::vector<ClientInfo>::iterator   find_nick(std::vector<ClientInfo>  &clients, std::string nick);
std::string                         get_nicks(std::vector<ClientInfo> &clients, Channel &channel);
std::string							get_allNicks(std::vector<ClientInfo> &clients);
std::string							getMessage(std::string oldMessage);
void								topic(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels);
void	                            part(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels);
void	                            who(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels);
void								cap(std::vector<ClientInfo>::iterator &it);
void								join(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels);
void                                notice(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels);
void								nick(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients);
void	                            pass(std::string message,std::vector<ClientInfo>::iterator &it, std::string passw);
void	                            ping(std::string command,std::vector<ClientInfo>::iterator &it);
void 	                            privmsg(std::string message, std::vector<ClientInfo>::iterator &it, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels);
void								quit(std::string message, std::vector<ClientInfo>::iterator &it, fd_set &rfds, std::vector<ClientInfo> &clients, std::map<std::string, Channel> &channels);
void								names(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels, std::vector<ClientInfo> &allClients);
void	                            user(std::string message,std::vector<ClientInfo>::iterator &it, std::string hostname);
void								mode(std::string message, std::vector<ClientInfo>::iterator &it, std::map<std::string, Channel> &channels);
int                                 isValidUser(std::string name, std::vector<ClientInfo> &clients);
std::vector<std::string>::iterator	find_channel(std::vector<std::string> &joined, std::string channel);



#endif