#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include "Client.hpp"

class Channel 
{
private:
	std::string				password;
	std::string 			topic;
	bool					topic_flag;
public:
	std::string 			name;
	std::vector<Client *>	clients;
	std::vector<Client *>	operators;

	void				addClient(Client *client);
	bool				isOperator(Client& client);
	bool				in_this_channel(Client& client);
	void				kickClient(Client& client);
	void				changeTopic(const std::string& topic);
	const std::string&	getPassword() const;
	const std::string&	getName() const;
	const std::string&	getTopic() const;

	Channel();
	Channel(std::string);
	Channel(const Channel& other);
	~Channel();

};


#endif
