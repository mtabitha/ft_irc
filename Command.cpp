#include "Command.hpp"
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>
#include <sstream>

void Command::execute()
{
    if (!prefix.empty() && prefix.compare(client.getNick()))
        return ;
    else if (command == "PASS")
            cmdPASS();
    else if (client.getPassword())
    {
        if (command == "NICK")
            cmdNICK();
        if (client.getNick().empty())
            ;
        else if (command == "USER")
            cmdUSER();
        if (!client.isRegistred())
            ;
        else
        {
            if (command == "JOIN")
                cmdJOIN();
            else if (command == "PART")
                cmdPART();
            else if (command == "TOPIC")
                cmdTOPIC();
            else if (command == "NAMES")
                cmdNAMES();
            else if (command == "LIST")
                cmdLIST();
            else if (command == "KICK")
                cmdKICK();
            else if (command == "PRIVMSG")
                cmdPRIVMSG();
        }
    }
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
        case (ERR_PASSWDMISMATCH):
            message += ":Password incorrect";
            break ;
        case (RPL_YOUREOPER):
            message += ":You are now an IRC operator";
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
        case (ERR_NOTONCHANNEL):
            message += channel->getName() + " :You're not on that channel";
            break ;
        case (RPL_NOTOPIC):
            message += channel->getName() + " :No topic is set";
            break ;
        case (RPL_TOPIC):
            message += channel->getName() + " :" + channel->getTopic();
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

void Command::cmdPASS()
{
    if (args.size() != 1)
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    if (client.getPassword())
        return (responce(ERR_ALREADYREGISTRED, &client, nullptr));
    if (args[0] == server.getPassword())
        client.setPassword(true);
}

void Command::cmdNICK()
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
                client.setNick(args[0]);
                return (responce(RPL_WELCOME, &client, nullptr));
            }
            else
                return (arg = args[0], responce(ERR_ERRONEUSNICKNAME, &client, nullptr));   
        }
    }
}

void Command::cmdUSER()
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

// QUIT

void Command::cmdJOIN()
{
    if (args.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    for (std::vector<std::string>::iterator ait = args.begin(); ait != args.end(); ++ait)
    {
        if ((*ait)[0] != '#')
            continue ;
        Channel *channel = server.findChannel(*ait);
        if (channel == nullptr)
        {
            channel = new Channel(*ait, &client);
            server.getChannels().push_back(channel);
        }
        else if (channel->in_this_channel(&client))
            continue ;
        else
            channel->addClient(&client);
        for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        {
            (*it)->socket.buf_write += ":" + client.getNick() + "!" + client.getUsername()
                                    + "@" + client.getHostname() + " " + command +
                                    " " + channel->getName() + "\r\n";
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

// вызов responce
void Command::cmdPART()
{
    if (args.empty())
        return (responce(ERR_NEEDMOREPARAMS, &client, nullptr));
    for(std::vector<std::string>::iterator it = args.begin()++; it != args.end(); ++it)
    {
        Channel *channel = server.findChannel(*it);
        if (!channel)
            (arg = *it, responce(ERR_NOSUCHCHANNEL, &client, nullptr));
        if (!channel->in_this_channel(&client))
            responce(ERR_NOTONCHANNEL, &client, channel);
        for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        {
            (*it)->socket.buf_write += ":" + client.getNick() + "!" + client.getUsername()
                                    + "@" + client.getHostname() + " " + command +
                                    " " + channel->getName() + "\r\n";
        }
        channel->kickClient(&client);
    }
}

void Command::cmdTOPIC()
{
    if (args.empty() || args.size() > 1) //убрать второе условие
        return ;
    Channel *channel = server.findChannel(args[0]);
    if (!channel)
        return ;
    if (!channel->in_this_channel(&client))
        return (responce(ERR_NOTONCHANNEL, &client, channel));
    if (text.empty())
    {
        if (channel->getTopic().empty())
            return (responce(RPL_NOTOPIC, &client, channel));
        return (responce(RPL_TOPIC, &client, channel));
    }
    if (!channel->isOperator(&client))
        return (responce(ERR_CHANOPRIVSNEEDED, &client, channel));
    channel->setTopic(text);
    for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
        responce(RPL_TOPIC,*it, channel);
}

void Command::cmdNAMES()
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

void Command::cmdLIST()
{
    // if (args.empty())
    //     return (RPL_LIST);
    // Channel *channel = server.findChannel(args[0]);
    // if (!channel)
    //     return (RPL_LISTEND);
    // return (RPL_LISTSTART); 
}

void Command::cmdKICK()
{
    // if (args.size() < 2)
    //     return (ERR_NEEDMOREPARAMS);
    // Channel *channel = server.findChannel(args[0]);
    // if (!channel)
    //     return (ERR_NOSUCHCHANNEL);
    // if (!channel->isOperator(&client))
    //     return (ERR_CHANOPRIVSNEEDED);
    // Client *other_client = server.findClient(args[1]);
    // if (!other_client)
    //     return (ERR_NOSUCHNICK);
    // channel->kickClient(other_client);
    // return (RPL_NO);
}
//или по nick или по названию канала 

void Command::cmdPRIVMSG()
{
    if (text.empty() && text.empty())
        return (responce(ERR_NOTEXTTOSEND, &client, nullptr));
    if (args.size() < 1)
        return (responce(ERR_NORECIPIENT, &client, nullptr));
    for(std::vector<std::string>::const_iterator cit = args.begin()++; cit != args.end(); ++cit)
	{
        Channel * channel = server.findChannel(*cit);
        Client * other_client = server.findClient(*cit);
        if (other_client != nullptr)
        {
            other_client->socket.buf_write +=   ":" + client.getNick() +
                                                "!" + client.getUsername() +
                                                "@" + client.getHostname() +
                                                " " + command +
                                                " " + other_client->getNick() +
                                                " " + text + "\r\n";
        }
        else if (channel != nullptr)
        {
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
            {
                if ((*it)->getNick() != client.getNick()) {
                    (*it)->socket.buf_write +=  ":" + client.getNick() +
                                                "!" + client.getUsername() +
                                                "@" + client.getHostname() +
                                                " " + command +
                                                " " + channel->getName() +
                                                " " + text + "\r\n";
                }
            }
        }
        else
        {
            arg = *cit;
            responce(ERR_NOSUCHNICK, &client, nullptr);
        }    
    }
}

const std::vector<std::string>&	Command::getArgs() const
{
    return (args);
}

void    Command::parser()
{
    std::istringstream ss(client.socket.buf_read);
    

    std::string buf;
    while (std::getline(ss, buf, ' '))
    {
        if (buf[0] == ':')
        {
            if (prefix.empty())
                setPrefix(buf);
            else    
                setText(buf);
        }
        else if (command.empty())
            setCommand(buf);
        else if (!command.empty())
        {
            std::istringstream ss1(buf);
            while (std::getline(ss1, buf, ','))
            {
                args.push_back(buf);
            }
            std::getline(ss, buf);
            if (buf[0] == ':')
                text = buf;
        }
    }
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


void		Command::printArgs(const std::vector<std::string>& args)
{
	for(std::vector<std::string>::const_iterator cit = args.begin()++; cit != args.end(); ++cit)
		std::cout << "\t" << *cit << std::endl;
}

std::ostream&	operator << (std::ostream& cout, const Command& cmd)
{
    std::cout 	<< "prefix : " << cmd.getPrefix() << std::endl
			        << "command: " << cmd.getCommand() << std::endl
			        << "args   : " << std::endl;
                    Command::printArgs(cmd.getArgs());
    std::cout       << "text   : "<< cmd.getText();
    return (cout);
}

Command::Command(Server& server, Client& client) : client(client), server(server)
{
    parse();
}

Command::~Command() {}

std::string toUpper(std::string token) {
	std::string upperString;

	for (std::string::size_type i = 0; i < token.length(); ++i)
    		upperString.push_back(std::toupper(token[i]));
	return upperString;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))	{
		if (token.compare("") && token.compare(" ")) {
			tokens.push_back(token);
        }
    }
	return tokens;
}

std::string checkPrefix(std::string message) {
	int		    pos1 = 0;
	int		    pos2 = 0;
	std::string prefix;

	if (message.compare(0, 1, ":") == 0) {
        pos1 = 1;
        pos2 = message.find(" ");
        if (pos2 != static_cast<int>(std::string::npos)) {
            return message.substr(pos1, pos2 - 1);
        }
	}
    return "";
}

void Command::parse() {
    std::string token;
    std::string buf(client.socket.buf_read);

    this->setPrefix(checkPrefix(client.socket.buf_read));
    if (!this->getPrefix().empty())
        buf = buf.erase(0, this->getPrefix().length() + 1);
	std::istringstream tokenStream(buf);
	while (std::getline(tokenStream, token, ' ')) {
        if (this->getCommand().empty()) {
            this->setCommand(toUpper(token));
        } else  if (token[0] != ':'){
            std::vector<std::string> buf_vec = split(token, ',');
            for(std::vector<std::string>::iterator it = buf_vec.begin(); it != buf_vec.end(); ++it)
                args.push_back(*it);
        }
        else
            break ;
    }
    size_t pos = buf.find(':');
    if (pos != std::string::npos)
        text = buf.substr(pos);
}
