#ifndef CHANNEL_HPP
# define CHANNEL_HPP

//# include <vector>
# include <string>
# include "Client.hpp"
#include "Server.hpp"
#include <set>

using namespace std;

class Channel 
{
private:
	string			password;
	string 			topic;
	bool			topic_flag;
public:
	string 			name;
	set<Client *>	clients;
	set<Client *>	operators;

	void		addClient(Client *client);
	bool			isOperator(Client *client);
	bool			in_this_channel(Client *client);
	void		kickClient(Client *client);
	void		setTopic(const string& topic);
	const string&	getPassword() const;
	const string&	getName() const;
	const string&	getTopic() const;
	string         getClientNames(void) const;
	Channel();
	Channel(string);
	Channel(const Channel& other);
	Channel(string name, Client *FirstChop);
	~Channel();

	void	part_delete_from_all_channels(Client *client);
	void		write_message_for_all(string message);
	void	name_of_all_clients_in_channel(Client *client);
};



#endif
