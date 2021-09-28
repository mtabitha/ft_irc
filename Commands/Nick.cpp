#include "Nick.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Nick::Nick(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void					Nick::printMessage(Client* other_client, Channel* channel)
{
    (void)(channel);
    other_client->socket.buf_write +=   ":" + client.getNick() + 
                                        " " + command +
                                        " " + args[0] + "\r\n";
}

void	                Nick::execute()
{
    if (args.empty())
        return (responce(ERR_NONICKNAMEGIVEN, &client, nullptr));
    else if (args.size() == 1)
    {
        if (args[0] == client.getNick())
            return (responce(ERR_NICKNAMEINUSE, &client, nullptr));
        else
        {
            if (isalpha(args[0][0]))
            {
                for(std::string::iterator it = args[0].begin()++; it != args[0].end(); ++it)
                    if (!isalnum(*it))
                        return (arg = args[0], responce(ERR_ERRONEUSNICKNAME, &client, nullptr));
                Client *other_client = server.findClient(args[0]);
                if (other_client)
                    return (arg = args[0], responce(ERR_NICKCOLLISION, &client, nullptr));
                if (!client.getNick().empty())
                    for (std::vector<Client *>::iterator cit = server.getClients().begin(); cit != server.getClients().end(); ++cit)
                        printMessage(*cit, nullptr);
                else
                    printMessage(&client, nullptr);
                client.setNick(args[0]);
            }
            else
                return (arg = args[0], responce(ERR_ERRONEUSNICKNAME, &client, nullptr));   
        }
    }
}

Nick::~Nick() {}