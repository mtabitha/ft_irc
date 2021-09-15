#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <string>

struct Network
{
    std::string			host;
	std::string			port;
	std::string 		password;
    Network (std::string, std::string, std::string);
};

#endif