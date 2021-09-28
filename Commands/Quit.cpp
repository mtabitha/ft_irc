#include "Quit.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"
#include "Part.hpp"

Quit::Quit(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Quit::execute()
{
    Part part(server, client, "PART");
    for (std::vector<Channel *>::iterator chit = server.getChannels().begin(); chit != server.getChannels().end(); ++chit)
        part.getArgs().push_back((*chit)->getName());
    part.execute();
    close(client.socket.socketfd);
	vector<Client *>::iterator it1 = find(server.getClients().begin(), server.getClients().end(), &client);
    delete &client;
    server.getClients().erase(it1);
}

Quit::~Quit() {}