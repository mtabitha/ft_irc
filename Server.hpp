#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <sys/_types/_size_t.h>
# include <sys/socket.h>
# include <sstream>
# include <vector>
# include <string>

# include "Client.hpp"
# include "network.hpp"

class Command;



# define BACKLOG 128
# define Xv(err,res,str)	(x_void(err,res,str,__FILE__,__LINE__))
# define X(err,res,str)		(x_int(err,res,str,__FILE__,__LINE__))

class Server {
	private:
		const Network			network;
		std::vector<Client *>	clients;
		std::vector<Channel *>	channels;

		static fd_set   readfds;
		static fd_set   writefds;
		int				maxfd;
		Command* 					parse(Client& client);
		Command* 					newCommand(Client& client, std::string str);
		void 						response(void);
		void						init_fd();
		void 						do_select();
		void 						check_sock();
	public:
		void						deleteClient(Client& client);

		Channel*					findChannel(const std::string channal_name);
		Client*						findClient(const std::string client_name);

		void 						startServer();
		const Network&				getNetwork() const;
		const std::string&			getPassword();
		std::vector<Client*>&		getClients(void);
		std::vector<Channel*>&		getChannels(void);
		
		Server(std::string host, std::string port, std::string password);
};

std::vector<std::string> split(const std::string& s, char delimiter);
int		x_int(int err, int res, const char *str, const char *file, int line);
void	*x_void(void *err, void *res, const char *str, const char *file, int line);

#endif
