#ifndef NAMES_HPP
# define NAMES_HPP

# include <iostream>
# include "Command.hpp"

class Names : public Command
{
	private:

	public:
		Names(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Names();
};

#endif 