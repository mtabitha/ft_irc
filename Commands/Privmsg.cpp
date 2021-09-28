#include "Privmsg.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Privmsg::Privmsg(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}
void					Privmsg::printMessage(Client* other_client, Channel* channel)
{

    other_client->socket.buf_write +=   ":" + client.getNick() +
                                        "!" + client.getUsername() +
                                        "@" + client.getHostname() +
                                        " " + command +
                                        " " + (!channel ? other_client->getNick() : channel->getName()) +
                                        " " + text + "\r\n";
}

void	                Privmsg::execute()
{
    if (text.empty() && text.empty())
            return (responce(ERR_NOTEXTTOSEND, &client, nullptr));
    if (args.size() < 1)
        return (responce(ERR_NORECIPIENT, &client, nullptr));
    std::vector<std::string> args1 = split(args[0], ',');
    for(std::vector<std::string>::const_iterator cit = args1.begin(); cit != args1.end(); ++cit)
    {
        Channel * channel = server.findChannel(*cit);
        Client * other_client = server.findClient(*cit);
        if (other_client != nullptr)
            printMessage(other_client, nullptr);
        else if (channel != nullptr)
        {
            if (channel->n_flag && !channel->in_this_channel(&client))
                continue ;
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
                if (client.getNick().compare((*it)->getNick()))
                    printMessage(*it, channel);
        }
        else
        {
            arg = *cit;
            responce(ERR_NOSUCHNICK, &client, nullptr);
        }    
    }
}

Privmsg::~Privmsg() {}