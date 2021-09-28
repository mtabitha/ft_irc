# include "Server.hpp"
# include "Commands/Join.hpp"
# include "Commands/Mode.hpp"
# include "Commands/Nick.hpp"
# include "Commands/Pass.hpp"
# include "Commands/Topic.hpp"
# include "Commands/Invite.hpp"
# include "Commands/Kick.hpp"
# include "Commands/Names.hpp"
# include "Commands/Part.hpp"
# include "Commands/Privmsg.hpp"
# include "Commands/User.hpp"
# include "Commands/Quit.hpp"

fd_set  Server::readfds;
fd_set  Server::writefds;

std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (token.compare("") && token.compare(" ")) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

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
	Client * serv = new Client(Socket::FD_SERVER, 
	X(-1, socket(result -> ai_family, result -> ai_socktype, result -> ai_protocol), "socket"));
	int opt = 1;
	maxfd = serv->socket.socketfd;
	setsockopt(serv->socket.socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	X(-1, bind(serv->socket.socketfd, result->ai_addr, result->ai_addrlen), "bind");
	X(-1, listen(serv->socket.socketfd, BACKLOG), "listen");
	clients.push_back(serv);
	freeaddrinfo(result);
}

Channel*			Server::findChannel(const std::string channal_name)
{
	for(std::vector<Channel *>::iterator chit = channels.begin(); chit != channels.end(); ++chit)
        if (channal_name == (*chit)->getName())
			return (*chit);
	return (nullptr);
}

Client*				Server::findClient(const std::string client_name)
{
	for(std::vector<Client *>::iterator cit = clients.begin(); cit != clients.end(); ++cit)
        if (client_name == (*cit)->getNick())
			return (*cit);
	return (nullptr);
}

void	Server::init_fd()
{
  	FD_ZERO(&readfds);
  	FD_ZERO(&writefds);

	for (std::vector<Client *>::const_iterator cit = clients.cbegin(); cit != clients.cend(); ++cit) 
		if ((*cit)->socket.type != Socket::FD_FREE)
		{		
	      	FD_SET((*cit)->socket.socketfd, &readfds);
	      	if (!(*cit)->socket.buf_write.empty())
	        	FD_SET((*cit)->socket.socketfd, &writefds);
		}
}

void Server::do_select()
{
	X(-1, select(maxfd + 1, &readfds, &writefds, NULL, NULL), "select");
}

void	writeWelcomeMessageToClient(Client *client)
{
	client->socket.buf_write = "Hello bro! Welcome to our IRC server.\n";
	client->socket.buf_write += "I hope you will enjoy it.\n";
	client->socket.buf_write += "Now you should write \'pass\' command\nand then through the space our password\nto sign in our server.\n";
	client->socket.buf_write += "Or you can write \'quit\' command to close your session.\n";


}



void Server::check_sock()
{
    if (FD_ISSET(clients[0]->socket.socketfd, &readfds))
    {
        struct sockaddr_in 	addr;
	    socklen_t			addr_size;

	    X(-1, fcntl(clients[0]->socket.socketfd, O_NONBLOCK), "fcntl");
	    addr_size = sizeof(addr);
		Client * client = new Client(Socket::FD_CLIENT, 
	    X(-1, accept(clients[0]->socket.socketfd , reinterpret_cast<struct sockaddr*>(&addr), &addr_size), "accept"));
		clients.push_back(client);
		writeWelcomeMessageToClient(client);
		maxfd = maxfd > client->socket.socketfd ? maxfd : client->socket.socketfd;
    }
	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) 
	{
		if (FD_ISSET((*it)->socket.socketfd, &readfds))
		{
            int ret = (*it)->socket.sock_read();
			std::cout << "Client " << (*it)->socket.socketfd << " : ";
			if (ret == 0)
			{
				close((*it)->socket.socketfd);
				std::cout << "DISCONNECT " << std::endl;
				(*it)->socket.buf_read = "QUIT";
				Command * cmd = newCommand(**it, "QUIT");
				cmd->preparationExecute();
				if (it == clients.end())
					break ;
			}
			else if (ret == -1 )
				std::cout << "CONNECT" << std::endl;
		}
		if (FD_ISSET((*it)->socket.socketfd, &writefds))
			(*it)->socket.sock_write();
	}
}

void Server::response(void)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->socket.buf_read.back() != '\n')
			continue ;
		std::istringstream ss((*it)->socket.buf_read);
		while (std::getline(ss, (*it)->socket.buf_read))
		{
			std::cout << (*it)->socket.buf_read << std::endl;
			if ( (*it)->socket.buf_read.back() == '\r')
				(*it)->socket.buf_read.pop_back();
			Command		*cmd = parse(**it);
			(*it)->socket.buf_read.clear();
			if (!cmd)
				continue ;
			std::cout << *cmd << std::endl;
			cmd->preparationExecute();
			delete cmd;
			if (it == clients.end())
				return ;
		}
	
	}
}

void Server::startServer() 
{
	while (true) 
	{
		init_fd();
      	do_select();
      	check_sock();
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

std::vector<Client *>& Server::getClients(void)
{
	return (clients);
}

std::vector<Channel *>&	Server::getChannels(void)
{
	return (channels);
}

std::string toUpper(std::string token) {
	std::string upperString;

	for (std::string::size_type i = 0; i < token.length(); ++i)
    		upperString.push_back(std::toupper(token[i]));
	return upperString;
}

std::string checkPrefix(std::string message) {
	int		    pos1 = 0;
	int		    pos2 = 0;
	std::string prefix;

	if (message.compare(0, 1, ":") == 0) {
        pos1 = 1;
        pos2 = message.find(" ");
        if (pos2 != static_cast<int>(std::string::npos)) {
            return message.substr(pos1, pos2 - 1);
        }
	}
    return "";
}

Command* Server::newCommand(Client& client, std::string str)
{
	if (str == "QUIT")
		return (new Quit(*this, client, str));
	else if (str == "PASS")
		return (new Pass(*this, client, str));
	else if (str == "NICK")
		return (new Nick(*this, client, str));
	else if (str == "USER")
		return (new User(*this, client, str));
	else if (str == "JOIN")
		return (new Join(*this, client, str));
	else if (str == "PART")
		return (new Part(*this, client, str));
	else if (str == "INVITE")
		return (new Invite(*this, client, str));
	else if (str == "TOPIC")
		return (new Topic(*this, client, str));
	else if (str == "NAMES")
		return (new Names(*this, client, str));
	else if (str == "KICK")
		return (new Kick(*this, client, str));
	else if (str == "PRIVMSG")
		return (new Privmsg(*this, client, str));
	else if (str == "MODE")
		return (new Mode(*this, client, str));
	return (nullptr);
}

Command* Server::parse(Client& client)
{
	std::string prefix;
    std::string token;
    std::string buf(client.socket.buf_read);
    Command *cmd = nullptr;
    prefix = (checkPrefix(client.socket.buf_read));
    if (!prefix.empty())
        buf = buf.erase(0, prefix.length() + 1);
	std::istringstream tokenStream(buf);
	while (std::getline(tokenStream, token, ' ')) {
        if (!cmd) {
			cmd = newCommand(client, toUpper(token));
            if (!cmd)
				return (nullptr);
        } else  if (token[0] != ':'){
            cmd->setArgs(token);
        }
        else
            break ;
    }
    size_t pos = buf.find(':');
    if (pos != std::string::npos)
        cmd->setText(buf.substr(pos));
	return (cmd);
}
