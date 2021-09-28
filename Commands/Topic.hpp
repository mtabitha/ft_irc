#ifndef TOPIC_HPP
# define TOPIC_HPP

# include <iostream>
# include "Command.hpp"

class Topic : public Command
{
	private:

	public:
		Topic(Server& server,Client& client, std::string str);
        virtual void	                execute();
		virtual                         ~Topic();
};

#endif 