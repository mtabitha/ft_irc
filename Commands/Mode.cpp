#include "Mode.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

void Mode::printMODE(std::string param, Channel& channel, std::string oper)
{
    for (std::set<Client *>::iterator it = channel.clients.begin(); it != channel.clients.end(); ++it)
    {
        (*it)->socket.buf_write +=  ":" + client.getNick() +
                                    "!" + client.getUsername() +
                                    "@" + client.getHostname() +
                                    " " + command +
                                    " " + channel.getName() +
                                    " " + oper + param + "\r\n";
    }
}

Mode::Mode(Server& server,Client& client, std::string str) : Command(server, client)
{
    setCommand(str);
}

void	                Mode::execute()
{
    if (args.size() < 2)
        return ;
    Channel *channel = server.findChannel(args[0]);
    if (!channel)
        return (responce(ERR_NOSUCHCHANNEL, &client, nullptr));
    if (!channel->isOperator(&client))
        return (responce(ERR_CHANOPRIVSNEEDED, &client, channel));
    std::vector<std::string>::iterator ait = ++args.begin();
    std::vector<std::string>::iterator oit = ++args.begin();
    std::string::iterator sit;
    std::string mods = "+-oltin";
    for(sit = ait->begin(); ait != args.end(); ++ait, sit = ait->begin())
    {
        if (*sit == '+' || *sit == '-')
        {
            char buf_c = *sit;
            sit++;
            std::string mod_c = "";
            for( ; sit != ait->end(); ++sit)
            { 
                if (*sit == '-' || *sit == '+')
                {
                    buf_c = *sit;
                    continue ; 
                }
                if (mods.find(*sit) == std::string::npos)
                    continue ;
                mod_c = *sit;
                if (*sit == 'o')
                {
                    for( ; oit != args.end(); ++oit)
                        if (isalpha((*oit)[0]))
                        {
                            Client * user = server.findClient(*oit++);
                            if (user != nullptr)
                            {
                                if (channel->in_this_channel(user))
                                    buf_c == '+' ? channel->setOper(user) : channel->delOper(user);
                                printMODE(" " + user->getNick(), *channel, buf_c + mod_c);
                            }
                            else
                                responce(ERR_NOSUCHNICK, &client, nullptr);
                            break ;
                        }
                    continue ;
                }
                else if (*sit == 'l')
                {
                    if (buf_c == '-')
                        channel->limit = -1;
                    else
                        for( ; oit != args.end(); ++oit)
                            if (isnumber((*oit)[0]))
                                {
                                    channel->setLimit(std::stoul(*oit));
                                    break ;
                                }
                    printMODE(" " + (*oit++), *channel, buf_c + mod_c);
                    continue ;
                }
                else if (*sit == 't')
                    buf_c == '+' ? channel->t_flag |= true : channel->t_flag &= false;
                else if (*sit == 'i')
                    buf_c == '+' ? channel->i_flag |= true : channel->i_flag &= false;
                else if (*sit == 'n')
                    buf_c == '+' ? channel->n_flag |= true : channel->n_flag &= false;
                printMODE("", *channel, buf_c + mod_c);
            }
        }
    }
}

Mode::~Mode() {}