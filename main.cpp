#include "Server.hpp"

std::vector<std::string>	isValid(int argc, char *argv[])
{
	std::vector<std::string>	args;

	if (argc < 2 || argc > 3)
		return (args);
	if (argc == 2)
	{
		std::string str(argv[1]);
		std::istringstream ss(str);
		std::string buf;

		if (str.back() == ':')
			return args;
		while (std::getline(ss, buf, ':'))
			args.push_back(buf);
		if (args.size() != 3)
			return (std::vector<std::string>());
	}
	else 
	{
		args.push_back("127.0.0.1");
		args.push_back(argv[1]);
		args.push_back(argv[2]);
	}
	return (args);
}

int 	main(int argc, char *argv[]) 
{
	std::vector<std::string> args;
	if ((args = isValid(argc, argv)).empty())
		return (std::cerr << "./ircserv [host:port_network:password_network] <port> <password>" << std::endl, 1);
	
	Server server(args[0], args[1], args[2]);
	server.startServer();
	
	return 0;
}
