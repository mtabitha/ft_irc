#include "Kick.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Kick::Kick(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void					Kick::printMessage(Client* other_client, Channel* channel)
{
    other_client->socket.buf_write +=   ":" + client.getNick() + 
                                        "!" + client.getUsername() + 
                                        "@" + client.getHostname() + 
                                        " " + command +
                                        " " + channel->getName() + 
                                        " " + args[1] + "\r\n";
}

void	                Kick::execute()
{
    if (args.size() < 2)
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (args.size() > 2)
        return ;
    Channel *channel = server.findChannel(args[0]);
    if (!channel)
        return (responce(ERR_NOSUCHCHANNEL, &client, nullptr));
    if (!channel->isOperator(&client))
        return (responce(ERR_CHANOPRIVSNEEDED, &client, channel));
    Client *other_client = server.findClient(args[1]);
    if (!other_client)
        return (arg = args[1], responce(ERR_NOSUCHNICK, &client, channel));
    for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        printMessage(*it, channel);
    channel->kickClient(other_client);
}

Kick::~Kick() {}