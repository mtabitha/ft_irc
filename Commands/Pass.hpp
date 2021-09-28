#ifndef PASS_HPP
# define PASS_HPP

class Server;

# include <iostream>
# include "Command.hpp"
# include "../Server.hpp"
# include "../Client.hpp"

class Pass : public Command
{
	private:

	public:
		Pass(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Pass();
};

#endif 