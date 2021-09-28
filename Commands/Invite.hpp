#ifndef INVITE_HPP
# define INVITE_HPP

# include <iostream>
# include "Command.hpp"

class Invite : public Command
{
	private:

	public:
		Invite(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual void					printMessage(Client* other_client, Channel* channel);
		virtual                         ~Invite();
};

#endif 