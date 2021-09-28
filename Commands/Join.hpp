#ifndef JOIN_HPP
# define JOIN_HPP

class Server;
class Client;
class Command;

# include "Command.hpp"

class Join : public Command
{
	private:

	public:
		Join(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Join();
};

#endif 