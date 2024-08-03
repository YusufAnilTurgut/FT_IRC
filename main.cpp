#include "Server.hpp"

int init_env(char **args,Server &serv)
{
	int	tmp;
	int	i = -1;
    while (args[1][++i])
	{
		if (!isdigit(args[1][i])) { std::cerr << "Invalid argument: port has non-digit characters" << std::endl; return (0); }
	}
	std::string	a(args[2]);
	if (a.empty()) { std::cerr << "Invalid argument: password is empty" << std::endl; return (0); }
	tmp = atoi(args[1]);
	if (!(tmp > 0 && tmp < 65537)) { std::cerr << "Invalid argument: port is not between 0 and 65537" << std::endl; return (0); }
	serv.set(a, tmp);
	return (1);
}

int main(int arc, char **args)
{
	Server	serv;
	if (arc != 3) { std::cerr << "Invalid number of arguments !" << std::endl; return(1); }
    if (!init_env(args, serv)) { return(2); }
	serv.setup();
	serv.irc_loop();

	return (0);
}
