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
		enum e_cmdType {NICK, USER, OPER, JOIN, PART,
						TOPIC, NAMES, LIST, KICK, PRIVMSG}
						cmdType; 
		enum e_resType { RPL_NO, RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO, RPL_ISSUPPORT,
						RPL_LISTSTART = 321, RPL_LIST, RPL_LISTEND, RPL_CHANNELMODEIS,
						RPL_NOTOPIC = 331, RPL_TOPIC, RPL_TOPICWHOTIME, RPL_NAMREPLY = 353,
						RPL_ENDOFNAMES = 366, RPL_YOUREOPER = 381, ERR_NOSUCHNICK = 401,
						ERR_NOSUCHCHANNEL = 403, ERR_CANNOTSENDTOCHAN, ERR_TOOMANYCHANNELS,
						ERR_WASNOSUCHNICK, ERR_TOOMANYTARGETS, ERR_NORECIPIENT = 414,
						ERR_NOTEXTTOSEND, ERR_NOTOPLEVEL, ERR_WILDTOPLEVEL, ERR_UNKNOWNCOMMAND = 421,
						ERR_NONICKNAMEGIVEN = 431, ERR_ERRONEUSNICKNAME, ERR_NICKNAMEINUSE,
						ERR_NICKCOLLISION = 436, ERR_USERNOTINCHANNEL = 441, ERR_NOTONCHANNEL,
						ERR_NEEDMOREPARAMS = 461, ERR_ALREADYREGISTRED, ERR_PASSWDMISMATCH = 464,
						ERR_CHANNELISFULL = 471, ERR_INVITEONLYCHAN = 473, ERR_BANNEDFROMCHAN,
						ERR_BADCHANNELKEY, ERR_CHANOPRIVSNEEDED = 482, ERR_NOOPERHOST = 491}
						resType;
		std::string					prefix;
		std::string					command;
		std::string					sender;
		std::string					message;
		std::vector<std::string>	args;
		std::string   				text;
		Client&						client;
		Server&						server;
		
		void			parser();
	public:
		void	execute();

		e_resType cmdPASS();
		e_resType cmdNICK();
		e_resType cmdUSER();
		e_resType cmdOPER();
		e_resType cmdJOIN();
		e_resType cmdPART();
		e_resType cmdTOPIC();
		e_resType cmdNAMES();
		e_resType cmdLIST();
		e_resType cmdKICK();
		e_resType cmdPRIVMSG();

		void    responce();

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

		//parser methods
		void 		parse();

		//void 		setPrefix(std::string);
		//std::string	getPrefix();

		//void		setCommand(std::string);
		//std::string	getCommand();

		//std::vector<std::string> &getArgs();

		void 		setMessage(std::string);
		std::string	getMessage();
		


};

		std::ostream&	operator << (std::ostream& cout, const Command& cmd);

#endif // !MESSAGE_HPP

		// case 437:		// ERR_
		// 	buff += ":Nick/channel is temporarily unavailable";
		// 	break;
		// case 476:		// ERR_
		// 	buff += ":Bad Channel Mask";
		// 	break;
		// case 477:		// ERR_
		// 	buff += ":Channel doesn't support modes";
		// 	break;
		// case 484:		// ERR_
		// 	buff += ":Your connection is restricted!";
		// 	break;
	