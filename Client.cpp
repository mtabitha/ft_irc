#include "Client.hpp"

void	Client::addChannal(Channel* channel)
{
    in_channel = channel;
}

void    Client::registration(std::vector<std::string>& args, std::string realname)
{
    username = args[0];
    hostname = args[1];
    servername = args[2];
    this->realname = realname;
    registration_flag = true;
}

bool	Client::isRegistred()
{
    return (registration_flag);
}

bool Client::getPassword() const
{
    return (password_flag);
}

const std::string&	Client::getRealname() const
{
    return (realname);
}

const std::string&	Client::getServername() const
{
    return (servername);
}


const std::string& Client::getNick() const
{
    return (nick);
}

const std::string&	Client::getUsername() const
{
    return (username);
}

const std::string&	Client::getHostname() const
{
    return (hostname);
}


void    Client::setPassword(bool pass)
{
    password_flag = pass;
}

void	Client::setNick(std::string nick)
{
    this->nick = nick;
}

Client::Client(Socket::Type type, int socketfd) : socket(Socket(type, socketfd))
{
    registration_flag = false;
    password_flag = false;
}

Client::Client(const Client& other)
{
    nick = other.getNick();
    username = other.getUsername();
    servername = other.getServername();
    realname = other.getRealname();
    hostname = other.getHostname();
    password_flag = other.getPassword();
    registration_flag = other.registration_flag;
    socket = other.socket;
    in_channel = other.in_channel;
}

Client::~Client() 
{}

Client *Client::getItem() {
    return this;
}