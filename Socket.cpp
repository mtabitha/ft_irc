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
