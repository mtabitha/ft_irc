#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <fcntl.h>
# include <sys/select.h>
# include <unistd.h>
# include <sys/types.h>
# include <string>

# define BUF_SIZE 512
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))

class Client;

struct Socket
{
    enum    Type { FD_FREE, FD_SERVER, FD_CLIENT } type;
    int     socketfd;
    std::string buf_read;
    std::string buf_write;

    int 	sock_read();
    void	sock_write();

    static fd_set   readfds;
	static fd_set   writefds;

    Socket();
    Socket(Type type, int socktfd);
    Socket(const Socket& other);
    ~Socket();
};

int	    x_int(int err, int res, const char *str, const char *file, int line);

# include "Client.hpp"

#endif
