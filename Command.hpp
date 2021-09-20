#ifndef COMMAND_HPP
# define COMMAND_HPP

class Server;

# include <iostream>
# include <string>
# include <vector>

# include "Server.hpp"
# include "Client.hpp"

class Command {
	private:
		enum e_resType { RPL_NO, RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO, RPL_ISSUPPORT,
						RPL_LISTSTART = 321, RPL_LIST, RPL_LISTEND, RPL_CHANNELMODEIS,
						RPL_NOTOPIC = 331, RPL_TOPIC, RPL_TOPICWHOTIME, RPL_NAMREPLY = 353,
						RPL_ENDOFNAMES = 366, RPL_YOUREOPER = 381, ERR_NOSUCHNICK = 401,
						ERR_NOSUCHCHANNEL = 403, ERR_CANNOTSENDTOCHAN, ERR_TOOMANYCHANNELS,
						ERR_WASNOSUCHNICK, ERR_TOOMANYTARGETS, ERR_NORECIPIENT = 411,
						ERR_NOTEXTTOSEND, ERR_UNKNOWNCOMMAND = 421,
						ERR_NONICKNAMEGIVEN = 431, ERR_ERRONEUSNICKNAME, ERR_NICKNAMEINUSE,
						ERR_NICKCOLLISION = 436, ERR_USERNOTINCHANNEL = 441, ERR_NOTONCHANNEL,
						ERR_NEEDMOREPARAMS = 461, ERR_ALREADYREGISTRED, ERR_PASSWDMISMATCH = 464,
						ERR_CHANNELISFULL = 471, ERR_INVITEONLYCHAN = 473, ERR_BANNEDFROMCHAN,
						ERR_BADCHANNELKEY, ERR_CHANOPRIVSNEEDED = 482, ERR_NOOPERHOST = 491};
		std::string					prefix;
		std::string					command;
		std::string					arg;
		std::vector<std::string>	args;
		std::string   				text;
		Client&						client;
		Server&						server;
		
		void			parser();
	public:
		void	execute();

		void cmdPASS();
		void cmdNICK();
		void cmdUSER();
		void cmdJOIN();
		void cmdTOPIC();
		void cmdNAMES();
		void cmdPRIVMSG();
		void cmdLIST();
		void cmdKICK();
		void cmdPART();
		void cmdQUIT();


		void    responce(Command::e_resType res, Client* Client, Channel* channel);
		void 	parse();

		void	setCommand(std::string cmd);
		void	setPrefix(std::string pref);
		void	setText(std::string text);

		const std::string&	getText() const;
		const std::string&	getCommand() const;
		const std::string&	getPrefix() const;
		const std::vector<std::string>&	getArgs() const;
		static void			printArgs(const std::vector<std::string>& args);

		Command(Server& server,Client& client);
		~Command();
};

		std::ostream&	operator << (std::ostream& cout, const Command& cmd);

#endif 
