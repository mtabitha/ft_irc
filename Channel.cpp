# include "Channel.hpp"
# include "Client.hpp"
# include <set>
# include <string>

void	Channel::part_delete_from_all_channels(Client *client)// вызываем эту функцию у каждого канала
{//															 в котором находим клиента(в сервере)
	//if (clients_on_channel.find(client) == clients_on_channel.end() && operators.find(client) == operators.end())
	//	return false;
	set<Client *>::iterator it = clients.find(client);
	clients.erase(it);
	//return true;
}

void	Channel::write_message_for_all(string message)
{
	set<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		(*it)->socket.buf_write = message; // вывести сообщения в сокеты всех клиентов канала
	}
	// set<Client *>::iterator it = operators.begin();
	// for (; it != operators.end(); it++)
	// {
	// 	(*it)->socket.buf_write = message; // вывести сообщения в сокеты всех клиентов канала
	// }
	
}

void	Channel::name_of_all_clients_in_channel(Client *client)
{
	string arr_name[clients.size()];
	int	i = 0;
	set<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++)
	{
		arr_name[i] = (*it)->getNick(); // реализовать метод
		i++;
	}
	// set<Client *>::iterator it = operators.begin();
	// for (; it != operators.end(); it++)
	// {
	// 	arr_name[i] = (*it)->getNick(); // реализовать метод
	// 	i++;
	// }
	for (int j = 0; j < i; j++)
	{
		client->socket.buf_write += (" " + arr_name[j]);// как то так нужно будет достучаться
		//									 до вывода на экран этого клиента
	}//											и вывести все
	client->socket.buf_write += "\n";

}

void			Channel::addInvitation(Client *client)
{
	invited.insert(client);
}

void			Channel::addClient(Client *client)
{
    clients.insert(client);
	invited.erase(client);
}

bool				Channel::isOperator(Client* client)
{
    if (operators.find(client) == operators.end())
		return false;
	return true;
}

std::string         Channel::getClientNames(void) const
{
    std::string names;
    for (std::set<Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
		if (operators.find(*it) != operators.end())
			names += '@';
		names += (*it)->getNick() + ' ';
    }
    return (names);
}

bool 			Channel::isInvited(Client *client)
{
	std::set<Client*>::iterator it = invited.find(client);
	if (it == invited.end())
		return (false);
	return (true);
}

bool				Channel::in_this_channel(Client* client)
{
    if (clients.find(client) == clients.end())
		return false;
	return true;
}

void			Channel::kickClient(Client *client)
{
	set<Client *>::iterator it = clients.find(client);
	set<Client *>::iterator it1 = operators.find(client);
	if (it1 != operators.end())
		operators.erase(it1);
	clients.erase(it);
}

void			Channel::setOper(Client *client)
{
	operators.insert(client);
}

void			Channel::delOper(Client *client)
{
	set<Client *>::iterator it1 = operators.find(client);
	if (it1 != operators.end())
		operators.erase(it1);
}

void				Channel::setTopic(const std::string& new_topic)
{
    topic = new_topic;
}

void			Channel::setLimit(size_t lim)
{
	limit = lim;
}

const std::string& Channel::getName() const
{
    return (name);
}

const std::string&	Channel::getTopic() const
{
    return (topic);
}

Channel::Channel()
{}

Channel::Channel(const Channel& other)
{
    name = other.getName();
    clients = other.clients;
    operators = other.operators;
    topic = other.getTopic();
}

Channel::~Channel()
{}

Channel::Channel(string name, Client *FirstChop, string password): name(name), password(password)
{
	limit = -1;
	t_flag = false;
	o_flag = false;
	i_flag = false;
	n_flag = false;
	l_flag = false;
	limit = INT_MAX;
	clients.insert(FirstChop);
	operators.insert(FirstChop);
}