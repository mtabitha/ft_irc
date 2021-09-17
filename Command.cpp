#include "Command.hpp"
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>
#include <sstream>

void Command::execute()
{
    if (!prefix.empty() && prefix.compare(client.getNick()))
        resType = RPL_NO;
    else if (command == "PASS")
        resType = cmdPASS();
    else if (client.getPassword())
    {
        if (command == "NICK")
            resType = cmdNICK();
        if (client.getNick().empty())
            ;
        else if (command == "USER")
            resType = cmdUSER();
        if (!client.isRegistred())
            ;
        else
        {
            if (command == "OPER")
                resType = cmdOPER();
            else if (command == "JOIN")
                resType = cmdJOIN();
            else if (command == "PART")
                resType = cmdPART();
            else if (command == "TOPIC")
                resType = cmdTOPIC();
            else if (command == "NAMES")
                resType = cmdNAMES();
            else if (command == "LIST")
                resType = cmdLIST();
            else if (command == "KICK")
                resType = cmdKICK();
            else if (command == "PRIVMSG")
                resType = cmdPRIVMSG();
        }
    }
    std::cout << resType << std::endl;
    responce();
}

void    Command::responce(Client* Client, Channel* channel)
{
   static std::string     message;
    message = ":"   +  server.getNetwork().host + " "
                    +   std::to_string(Command::resType) + " "
                    +   (Client->getNick().empty() ? "*" : Client->getNick()) + " ";
    if (RPL_NO)
        return ;
    if (resType == RPL_WELCOME && !command.compare("USER"))
        message += "Welcome to the Internet Relay Network " + Client->getNick()
                + "!" + Client->getUsername() + "@" + Client->getHostname();
    else if (resType == RPL_WELCOME)
        message += "Your nick now is " + Client->getNick() + "!";
    else if (resType == ERR_NONICKNAMEGIVEN)
        message += ":No nickname given";
    else if (resType == ERR_ERRONEUSNICKNAME)
        message += arg + " :Erroneus nickname";
    else if (resType == ERR_NICKNAMEINUSE)
        message += Client->getNick() + " :Nickname is already in use";
    else if (resType == ERR_NICKCOLLISION)
        message += arg + " :Nickname collision KILL";
    else if (resType == ERR_NEEDMOREPARAMS)
        message += command + " :Not enough parameters";
    else if (resType == ERR_ALREADYREGISTRED)
        message += ":You may not register";
    else if (resType == ERR_PASSWDMISMATCH)
        message += ":Password incorrect";
    else if (resType == RPL_YOUREOPER)
        message += ":You are now an IRC operator";
    else if (resType == ERR_NOSUCHCHANNEL)
        message += channel->getName() + " :No such channel";
    else if (resType == ERR_NOTONCHANNEL)
        message += channel->getName() + " :You're not on that channel";
    else if (resType == RPL_NOTOPIC)
        message += channel->getName() + " :No topic is set";
    else if (resType == ERR_CHANOPRIVSNEEDED)
        message += channel->getName() + " :You're not channel operator";
    else if (resType == RPL_TOPIC)
        message += channel->getName() + " :" + channel->getTopic();
    else if (resType == RPL_NAMREPLY)
        message += "= " + channel->getName() + " :" + channel->getClientNames();
    else if (resType == RPL_ENDOFNAMES)
        message += (channel == nullptr ? arg : channel->getName()) + " :End of /NAMES list";
    else if (resType == ERR_NOSUCHNICK)
        message += arg + " :No such nick/channel";
    if (resType != RPL_NO)
    {
        Client->socket.buf_write += message + "\r\n";
        message.clear();
    }
}

Command::e_resType Command::cmdPASS()
{
    if (args.size() != 1)
        return (ERR_NEEDMOREPARAMS);
    if (client.getPassword())
        return (ERR_ALREADYREGISTRED);
    if (args[0] == server.getPassword())
        client.setPassword(true);
    return (RPL_NO);
}

Command::e_resType Command::cmdNICK()
{
    if (args.empty())
        return (ERR_NONICKNAMEGIVEN);
    else if (args.size() == 1)
    {
        if (args[0] == client.getNick())
            return (ERR_NICKNAMEINUSE);
        else
        {
            if (isalpha(args[0][0]))
            {
                for(std::string::iterator it = args[0].begin()++; it != args[0].end(); ++it)
                    if (!isalnum(*it))
                        return (ERR_ERRONEUSNICKNAME);
                Client *other_client = server.findClient( args[0]);
                if (other_client)
                    return (ERR_NICKCOLLISION);
                client.setNick(args[0]);
                return (RPL_WELCOME);
            }
            else
                return (ERR_ERRONEUSNICKNAME);   
        }
    }
    else 
        return (ERR_NEEDMOREPARAMS);
}

Command::e_resType Command::cmdUSER()
{
    if (args.size() < 3 || text.empty())
        return (ERR_NEEDMOREPARAMS);
    if (args.size() > 3)
        return (RPL_NO);
    else if (client.isRegistred())
        return (ERR_ALREADYREGISTRED);
    client.registration(args, text);
    return (RPL_WELCOME);
}

Command::e_resType Command::cmdOPER()
{
    if (args.size() < 2)
        return (ERR_NEEDMOREPARAMS);
    if (args.size() > 2)
        return (RPL_NO);
    if (args[1] != client.in_channel->getPassword())
        return (ERR_PASSWDMISMATCH);
    client.setOper();
    return (RPL_YOUREOPER);
}

// QUIT

Command::e_resType Command::cmdJOIN()
{
    if (args.empty())
        return (ERR_NEEDMOREPARAMS);
    Channel *channel = server.findChannel(args[0]);
    if (channel == nullptr)
    {
        Channel * chan = new Channel(args[0]);
        server.getChannels().push_back(chan);
        channel = server.findChannel(args[0]);
    }
    channel->addClient(&client);
    for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
    {
        (*it)->socket.buf_write = ":" + client.getNick() + "!" + client.getUsername()
                                + "@" + client.getHostname() + " " + command +
                                " " + channel->getName() + "\r\n";
        if (*it == &client)
        {
            (*it)->socket.buf_write += ":"  + server.getNetwork().host + " " + std::to_string(RPL_NAMREPLY) + " "
                                            + client.getNick() + " = " + channel->getName() + " " + ":"
                                            + channel->getClientNames() + "\r\n";
            (*it)->socket.buf_write += ":"  + server.getNetwork().host + " " + std::to_string(RPL_ENDOFNAMES) + " " 
                                            + client.getNick() + " " + channel->getName() + " " + ":" 
                                            + "End of NAMES list" + "\r\n";                                   
        }
    }
    client.addChannal(channel);
    return (RPL_NO);
}

// вызов responce
Command::e_resType Command::cmdPART()
{
    if (args.empty())
        return (ERR_NEEDMOREPARAMS);
    for(std::vector<std::string>::iterator it = args.begin()++; it != args.end(); ++it)
    {
        Channel *channel = server.findChannel(*it);
        if (!channel)
            return (ERR_NOSUCHCHANNEL);
        if (!channel->in_this_channel(&client))
            return (ERR_NOTONCHANNEL);
        channel->kickClient(&client);
        return (RPL_NO);
    }
}

Command::e_resType Command::cmdTOPIC()
{
    if (args.empty() || args.size() > 2)
        return (ERR_NEEDMOREPARAMS);
    Channel *channel = server.findChannel(args[0]);
    if (!channel || !channel->in_this_channel(&client))
        return (ERR_NOTONCHANNEL);
    if (args.size() == 1 && channel->getTopic().empty())
        return (RPL_NOTOPIC);
    if (args.size() == 2)
    {
        if (!channel->isOperator(&client))
            return (ERR_CHANOPRIVSNEEDED);
        channel->setTopic(args[1]);
    }
    return (RPL_TOPIC);
}

Command::e_resType Command::cmdNAMES()
{
    
    vector<Channel *>::iterator chan_it = server.getChannels().begin();
    if (args.empty())
    {
        if (chan_it != server.getChannels().end())
        {
            for (; chan_it != server.getChannels().end(); chan_it++)
            {
                client.socket.buf_write += (*chan_it)->getName() + ":";
                (*chan_it)->name_of_all_clients_in_channel(&client); // тут сразу ставится запятая и выводятся ники
            }
        }
        else
        {
            client.socket.buf_write += ":"  + server.getNetwork().host + " " + std::to_string(RPL_ENDOFNAMES) + " " 
                                            + client.getNick() + " " + (*chan_it)->getName() + " " + ":" 
                                            + "End of NAMES list" + "\r\n";
        }
    }
    else
        for (int i = 0; i < args.size(); i++)
        {
            for (; chan_it != server.getChannels().end(); chan_it++)
            {
                if ((*chan_it)->getName() == args[i])
                {
                    client.socket.buf_write += ":" + client.getNick() + "!" + client.getUsername()
                                            + "@" + client.getHostname() + " " + command +
                                            " " + (*chan_it)->getName() + "\r\n";
                    client.socket.buf_write += ":"  + server.getNetwork().host + " " + std::to_string(RPL_NAMREPLY) + " "
                                            + client.getNick() + " = " + (*chan_it)->getName() + " " + ":"
                                            + (*chan_it)->getClientNames() + "\r\n";

                    client.socket.buf_write += ":"  + server.getNetwork().host + " " + std::to_string(RPL_ENDOFNAMES) + " " 
                                            + client.getNick() + " " + (*chan_it)->getName() + " " + ":" 
                                            + "End of NAMES list" + "\r\n";
                    break;
                }
            }
        }
    return (RPL_NO);
    // Channel *channel = server.findChannel(args[0]);
    // if (!channel || !channel->in_this_channel(&client))
    //     return (RPL_ENDOFNAMES);
    // channel->name_of_all_clients_in_channel(&client);
    // return (RPL_NAMREPLY);
}

Command::e_resType Command::cmdLIST()
{
    if (args.empty())
        return (RPL_LIST);
    Channel *channel = server.findChannel(args[0]);
    if (!channel)
        return (RPL_LISTEND);
    return (RPL_LISTSTART); 
}

Command::e_resType Command::cmdKICK()
{
    if (args.size() < 2)
        return (ERR_NEEDMOREPARAMS);
    Channel *channel = server.findChannel(args[0]);
    if (!channel)
        return (ERR_NOSUCHCHANNEL);
    if (!channel->isOperator(&client))
        return (ERR_CHANOPRIVSNEEDED);
    Client *other_client = server.findClient(args[1]);
    if (!other_client)
        return (ERR_NOSUCHNICK);
    channel->kickClient(other_client);
    return (RPL_NO);
}
//или по nick или по названию канала 
Command::e_resType Command::cmdPRIVMSG()
{
    if (text.empty())
        return (ERR_NOTEXTTOSEND);
    if (args.size() < 1)
        return (ERR_NORECIPIENT);
    for(std::vector<std::string>::const_iterator cit = args.begin()++; cit != args.end(); ++cit)
	{
        Channel * channel = server.findChannel(*cit);
        Client * other_client = server.findClient(*cit);
        if (other_client != nullptr)
        {
            other_client->socket.buf_write =    ":" + client.getNick() +
                                                "!" + client.getUsername() +
                                                "@" + client.getHostname() +
                                                " " + command +
                                                " " + other_client->getNick() +
                                                " " + text + "\r\n";
        }
        else if (channel != nullptr)
        {
            if (client.in_channel == nullptr) {
                return (ERR_NOTONCHANNEL);
            }
            for (std::set<Client *>::iterator it = channel->clients.begin(); it != channel->clients.end(); ++it)
            {
                if ((*it)->getNick() != client.getNick()) {
                    (*it)->socket.buf_write =   ":" + client.getNick() +
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
            resType = ERR_NOSUCHNICK;
            responce();
        }    
    }
    return (RPL_NO);
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
    resType = RPL_NO;
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

void Command::setMessage(std::string message) {
    this->message = message;
}

std::string Command::getMessage() {
    return this->message;
}
