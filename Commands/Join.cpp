# include "Join.hpp"
# include "../Server.hpp"
# include "../Client.hpp"
# include "Command.hpp"

Join::Join(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Join::execute()
{
    if (args.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    std::vector<std::string> args1 = split(args[0], ',');
    std::vector<std::string> second_args;
    if (args.size() == 1)
        second_args.clear();
    else
        second_args = split(args[1], ',');
    for (std::vector<std::string>::iterator ait = args1.begin(), sit = second_args.empty() ? second_args.end() : second_args.begin(); ait != args1.end(); ++ait)
    {
        std::string password = sit == second_args.end() ? "" : *sit++;
        if ((*ait)[0] != '#')
            continue ;
        Channel *channel = server.findChannel(*ait);
        if (channel == nullptr)
        {
            channel = new Channel(*ait, &client, password);
            server.getChannels().push_back(channel);
        }
        else if (!channel->isInvited(&client) && (channel->in_this_channel(&client) || ( channel->limit > 0 && (channel->clients.size() + 1 > channel->limit)) || channel->i_flag || channel->password != password || (channel->i_flag && channel->password == password)))
            continue ;
        else
            channel->addClient(&client);
        for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        {
            printMessage(*it, channel);
            if (*it == &client)
            {
                channel->getTopic().empty() ? responce(RPL_NOTOPIC, &client, channel)
                                          : responce(RPL_TOPIC, &client, channel);
                responce(RPL_NAMREPLY, &client, channel); 
                responce(RPL_ENDOFNAMES, &client, channel);                             
            }
        }
        client.addChannal(channel);
    }
}

Join::~Join() {}