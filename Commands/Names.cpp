#include "Names.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Names::Names(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Names::execute()
{
    if (args.empty())
        for (std::vector<Channel *>::iterator it = server.getChannels().begin(); it != server.getChannels().end(); ++it)
        {
            responce(RPL_NAMREPLY, &client, *it);
            responce(RPL_ENDOFNAMES, &client, *it);

        }
    else
        for (std::vector<std::string>::iterator ait = args.begin(); ait != args.end(); ++ait)
        {
            Channel *channel = server.findChannel(*ait);
            if (channel == nullptr)
            {
                arg = *ait;
                responce(RPL_ENDOFNAMES, &client, nullptr);
                continue;
            }
            responce(RPL_NAMREPLY, &client, channel);
            responce(RPL_ENDOFNAMES, &client, channel);
        }
}

Names::~Names() {}