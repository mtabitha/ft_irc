#include "User.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

User::User(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                User::execute()
{
    if (args.size() < 3 || text.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (args.size() > 3)
        return (responce(RPL_NO, &client, nullptr));
    else if (client.isRegistred())
        return (responce(ERR_ALREADYREGISTRED, &client, nullptr));
    client.registration(args, text);
    return (responce(RPL_WELCOME, &client, nullptr));
}

User::~User() {}