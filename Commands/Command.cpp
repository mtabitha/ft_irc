#include "Command.hpp"
#include "../Server.hpp"
#include "../Client.hpp"
#include "../Channel.hpp"

Command::Command(Server& server,Client& client) : client(client), server(server)
{}

Command::~Command()
{

}

void							Command::preparationExecute()
{
    if (command == "PASS" || command == "QUIT")
        ;
    else if (command == "NICK" && !client.password_flag)
        return ;
    else if (command == "USER" && client.getNick().empty())
        return ;
    else if (command != "NICK" && command != "USER" && !client.isRegistred())
        return ; 
    execute();
}

void    Command::setArgs(std::string args)
{
    this->args.push_back(args);
}

void	Command::setCommand(std::string cmd)
{
    command = cmd;
}

void	Command::setPrefix(std::string pref)
{
    prefix = pref;
}

void	Command::setText(std::string text)
{
    this->text = text;
}

const std::string&	Command::getCommand() const
{
    return (command);
}

const std::string&	Command::getPrefix() const
{
    return (prefix);
}

const std::string&	Command::getText() const
{
    return (text);
}

std::vector<std::string>&	Command::getArgs()
{
    return (args);
}

void	Command::printMessage(Client* other_client, Channel* channel)
{
    other_client->socket.buf_write +=   ":" + client.getNick() + 
                                        "!" + client.getUsername() + 
                                        "@" + client.getHostname() + 
                                        " " + command +
                                        " " + channel->getName() + "\r\n";
}

void    Command::responce(Command::e_resType res, Client* Client, Channel* channel)
{
    std::string     message;

    message = ":"   +  server.getNetwork().host + " "
                    +   std::to_string(res) + " "
                    +   (Client->getNick().empty() ? "*" : Client->getNick()) + " ";

    switch (res)
    {
        case (RPL_NO):
            return ;
        case (RPL_WELCOME):
            if (command.compare("USER"))
                message += "Your nick now is " + Client->getNick() + "!";
            else
                message += "Welcome to the Internet Relay Network " + Client->getNick()
                        + "!" + Client->getUsername() + "@" + Client->getHostname();
            break ;
        case (ERR_NONICKNAMEGIVEN):
            message += ":No nickname given";
            break ;
        case (ERR_ALREADYREGISTRED):
            message += ":You may not register";
            break ;
        case (ERR_NOTEXTTOSEND):
            message += " :No text to send";
            break ;
        case (ERR_NORECIPIENT):
            message += " :No recipient given " + command;
            break ;
        case (ERR_NEEDMOREPARAMS):
            message += command + " :Not enough parameters";
            break ;
        case (ERR_ERRONEUSNICKNAME):
            message += arg + " :Erroneus nickname";
            break ;
        case (ERR_NICKCOLLISION):
            message += arg + " :Nickname collision KILL";
            break ;
        case (ERR_NOSUCHCHANNEL):
            message += arg + " :No such channel";
            break ;
        case (ERR_NOSUCHNICK):
            message += arg + " :No such nick/channel";
            break ;
        case (ERR_NICKNAMEINUSE):
            message += Client->getNick() + " :Nickname is already in use";
            break ;
        case (ERR_USERONCHANNEL):
            message += arg + " " + channel->getName() + " :is already on channel";
            break ;
        case (ERR_NOTONCHANNEL):
            message += channel->getName() + " :You're not on that channel";
            break ;
        case (RPL_NOTOPIC):
            message += channel->getName() + " :No topic is set";
            break ;
        case (RPL_TOPIC):
            message += channel->getName() + " :" + channel->getTopic();
            break ;
        case (RPL_INVITING):
            message += channel->getName() + " " + arg;
            break ;
        case (ERR_CHANOPRIVSNEEDED):
            message += channel->getName() + " :You're not channel operator";
            break ;
        case (RPL_NAMREPLY):
            message += "= " + channel->getName() + " :" + channel->getClientNames();
            break ;
        case (RPL_ENDOFNAMES):
            message += (channel == nullptr ? arg : channel->getName()) + " :End of /NAMES list";
            break ;
    }
    Client->socket.buf_write += message + "\r\n";
    message.clear();
}

void		Command::printArgs(const std::vector<std::string>& args)
{
	for(std::vector<std::string>::const_iterator cit = args.begin()++; cit != args.end(); ++cit)
		std::cout << "\t" << *cit << std::endl;
}

std::ostream&	operator << (std::ostream& cout, Command& cmd)
{
    std::cout 	<< "prefix : " << cmd.getPrefix() << std::endl
			        << "command: " << cmd.getCommand() << std::endl
			        << "args   : " << std::endl;
                    Command::printArgs(cmd.getArgs());
    std::cout       << "text   : "<< cmd.getText();
    return (cout);
}