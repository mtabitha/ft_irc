#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include "Command.hpp"

class User : public Command
{
	private:

	public:
		User(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~User();
};

#endif 