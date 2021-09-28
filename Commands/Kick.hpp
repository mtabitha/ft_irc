#ifndef KICK_HPP
# define KICK_HPP

# include <iostream>
# include "Command.hpp"

class Kick : public Command
{
	private:

	public:
		Kick(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual void					printMessage(Client* other_client, Channel* channel);
		virtual                         ~Kick();
};

#endif 