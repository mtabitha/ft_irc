#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Client;
# include <set>
# include <string>
# include <iostream>
using namespace std;

class Channel 
{
private:
	string 			topic;
public:
	size_t			limit;
	bool			t_flag;
	bool			o_flag;
	bool			i_flag;
	bool			n_flag;
	bool			l_flag;
	string 			name;
	string		password;
	set<Client *>	clients;
	set<Client *>	operators;
	set<Client *>	invited;

	void			addInvitation(Client *client);
	void			addClient(Client *client);
	bool 			isInvited(Client *client);
	bool			isOperator(Client *client);
	bool			in_this_channel(Client *client);
	void			kickClient(Client *client);
	void			setTopic(const string& topic);
	void			setOper(Client *client);
	void			delOper(Client *client);
	void			setLimit(size_t lim);
	const string&	getName() const;
	const string&	getTopic() const;
	string         	getClientNames(void) const;
	Channel();
	Channel(string);
	Channel(const Channel& other);
	Channel(string name, Client *client, string password);
	~Channel();

	void	part_delete_from_all_channels(Client *client);
	void		write_message_for_all(string message);
	void	name_of_all_clients_in_channel(Client *client);
};



#endif
