#include "Socket.hpp"
# include <iostream>

fd_set  Socket::readfds;
fd_set  Socket::writefds;

Socket::Socket() 
{
    type = FD_FREE;
    socketfd = -1;
}

Socket::Socket(Type type, int socketfd)
{
    this->type = type;
    this->socketfd = socketfd;
}

int	Socket::sock_read()
{
    int 	nbytes;
	char	c_buf_read[BUF_SIZE + 1];

    memset(c_buf_read, 0, BUF_SIZE);
	nbytes = recv(socketfd, c_buf_read, sizeof(c_buf_read), 0);
    buf_read += c_buf_read;
    return (nbytes);
}

void	Socket::sock_write()
{
    int nbytes;

	nbytes = send(socketfd, buf_write.c_str(), strlen(buf_write.c_str()), 0);
	buf_write.clear();
}

void	Socket::init_fd(const std::vector<Client*>& clients)
{
  	FD_ZERO(&readfds);
  	FD_ZERO(&writefds);

	for (std::vector<Client *>::const_iterator cit = clients.cbegin(); cit != clients.cend(); ++cit) 
		if ((*cit)->socket.type != FD_FREE)
		{		
	      	FD_SET((*cit)->socket.socketfd, &readfds);
	      	if (!(*cit)->socket.buf_write.empty())
	        	FD_SET((*cit)->socket.socketfd, &writefds);
		}
}

void Socket::do_select(const std::vector<Client*>& clients)
{
	X(-1, select(clients.size() + 3, &readfds, &writefds, NULL, NULL), "select");
}

void Socket::check_sock(std::vector<Client*>& clients)
{
    if (FD_ISSET(clients[0]->socket.socketfd, &readfds))
    {
        struct sockaddr_in 	addr;
	    socklen_t			addr_size;

	    X(-1, fcntl(clients[0]->socket.socketfd, O_NONBLOCK), "fcntl");
	    addr_size = sizeof(addr);
	    clients.push_back(new Client(FD_CLIENT, 
	    X(-1, accept(clients[0]->socket.socketfd , reinterpret_cast<struct sockaddr*>(&addr), &addr_size), "accept")));
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
				delete *it;
				clients.erase(it);
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

Socket::Socket(const Socket& other)
{	
	type = other.type;
	socketfd = other.socketfd;
	buf_read = other.buf_read;
	buf_write = other.buf_write;
}

Socket::~Socket()
{
}
