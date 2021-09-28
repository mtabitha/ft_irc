#include "Part.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Part::Part(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Part::execute()
{
    if (args.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    std::vector<std::string> args1 = split(args[0], ',');
    for(std::vector<std::string>::iterator it = args1.begin(); it != args1.end(); ++it)
    {
        Channel *channel = server.findChannel(*it);
        if (!channel)
            (arg = *it, responce(ERR_NOSUCHCHANNEL, &client, nullptr));
        else if (!channel->in_this_channel(&client))
            responce(ERR_NOTONCHANNEL, &client, channel);
        else
        {
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
                printMessage(*it, channel);
            channel->kickClient(&client);
        }
    }
}

Part::~Part() {}