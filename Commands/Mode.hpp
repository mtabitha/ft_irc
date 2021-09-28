#ifndef MODE_HPP
# define MODE_HPP

# include <iostream>
# include "Command.hpp"

class Mode : public Command
{
	private:

	public:
		void 	printMODE(std::string param, Channel& channel, std::string oper);
		Mode(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Mode();
};

#endif 