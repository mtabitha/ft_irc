#include "Channel.hpp"

void				Channel::addClient(Client *client)
{
    clients.push_back(client);

}

bool				Channel::isOperator(Client& client)
{

}

bool				Channel::in_this_channel(Client& client)
{

}

void				Channel::kickClient(Client& client)
{

}

void				Channel::changeTopic(const std::string& topic)
{
    
}

const std::string& Channel::getPassword() const
{
    return (password);
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

Channel::Channel(std::string name)
{
    this->name = name;
}

Channel::Channel(const Channel& other)
{
    name = other.getName();
    clients = other.clients;
    operators = other.operators;

    password = other.getPassword();
    topic = other.getTopic();
}

Channel::~Channel()
{}
