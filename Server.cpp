#include "Server.hpp"
#include <sys/_types/_size_t.h>
#include <sys/socket.h>
#include <sstream>

Server::Server(std::string host, std::string port, std::string password) : 
		network(host, port, password)
{
	struct addrinfo	hints;
	struct addrinfo	*result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_PASSIVE;    /* for wildcard IP address */

	int ret = getaddrinfo(network.host.c_str(), network.port.c_str(), &hints, &result);
	if (ret != 0)
	{
		std::cerr << "Server: getaddrinfo error" << std::endl;
		exit(EXIT_FAILURE);
	}
	clients.clear();
	Client	serv(Socket::FD_SERVER, 
		X(-1, socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol), "socket"));
	int opt = 1;
	setsockopt(serv.socket.socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	X(-1, bind(serv.socket.socketfd, result->ai_addr, result->ai_addrlen), "bind");
	X(-1, listen(serv.socket.socketfd, BACKLOG), "listen");
	clients.push_back(serv);
	freeaddrinfo(result);
}

void Server::response(void)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		std::istringstream ss(it->socket.buf_read);
		while (std::getline(ss, it->socket.buf_read))
		{
			std::cout << it->socket.buf_read << std::endl;
			if ( it->socket.buf_read.back() == '\r')
				it->socket.buf_read.pop_back();
			Command		cmd(*this, *it);
			std::cout << cmd << std::endl;
			cmd.execute();
			it->socket.buf_read.clear();
		}
	
	}
}

Channel*			Server::findChannel(Server& server, const std::string channal_name)
{
	for(std::vector<Channel>::iterator chit = server.getChannels().begin(); chit != server.getChannels().end(); ++chit)
        if (channal_name == chit->getName())
			return (&(*chit));
	return (nullptr);
}

Client*				Server::findClient(Server& server, const std::string client_name)
{
	for(std::vector<Client>::iterator cit = server.getClients().begin(); cit != server.getClients().end(); ++cit)
        if (client_name == cit->getNick())
			return (&(*cit));
	return (nullptr);
}

void Server::startServer() 
{
	while (true) 
	{
		Socket::init_fd(clients);
      	Socket::do_select(clients);
      	Socket::check_sock(clients);
		response();
	}
}

const Network&				Server::getNetwork() const
{
	return (network);
}

const std::string&	Server::getPassword()
{
	return (network.password);
}

std::vector<Client>& Server::getClients(void)
{
	return (clients);
}

std::vector<Channel>&	Server::getChannels(void)
{
	return (channels);
}
