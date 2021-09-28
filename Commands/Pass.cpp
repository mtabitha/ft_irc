#include "Pass.hpp"

Pass::Pass(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Pass::execute()
{
    if (args.size() != 1)
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (client.getPassword())
        return (responce(ERR_ALREADYREGISTRED, &client, nullptr));
    if (args[0] == server.getPassword())
    {
        client.setPassword(true);
        client.socket.buf_write = "Now you should write nick. Show me your fantasy!\n";
        client.socket.buf_write += "To do this you need write \'nick\' command and your nick.\n";
    }
    else
        client.socket.buf_write = "Password is incorrect! Try again!\n";
}

Pass::~Pass() {}