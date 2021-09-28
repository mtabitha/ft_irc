#ifndef PART_HPP
# define PART_HPP

# include <iostream>
# include "Command.hpp"

class Part : public Command
{
	private:

	public:
		Part(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Part();
};

#endif 