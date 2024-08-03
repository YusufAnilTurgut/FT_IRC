#include "Server.hpp"

int count_stream(std::string str)
{
	std::stringstream ss(str);
	std::string word;
	int count = 0;
	while (ss >> word)
		count++;
	return count;
}
