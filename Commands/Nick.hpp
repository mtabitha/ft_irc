#ifndef NICK_HPP
# define NICK_HPP

# include <iostream>
# include "Command.hpp"

class Nick : public Command
{
	private:

	public:
		Nick(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual void					printMessage(Client* other_client, Channel* channel);
		virtual                         ~Nick();
};

#endif 