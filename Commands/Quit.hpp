#ifndef QUIT_HPP
# define QUIT_HPP

# include <iostream>
# include "Command.hpp"

class Quit : public Command
{
	private:

	public:
		Quit(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Quit();
};

#endif 