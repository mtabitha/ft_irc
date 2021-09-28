#ifndef COMMAND_HPP
# define COMMAND_HPP

class Server; 
class Client;
class Channel;

# include <iostream>
# include <vector>
# include <string>

class Command {
	protected:
		enum e_resType { RPL_NO, RPL_WELCOME, 
						RPL_NOTOPIC = 331, RPL_TOPIC, RPL_INVITING = 341, RPL_NAMREPLY = 353,
						RPL_ENDOFNAMES = 366, ERR_NOSUCHNICK = 401,
						ERR_NOSUCHCHANNEL = 403, ERR_NORECIPIENT = 411,
						ERR_NOTEXTTOSEND,
						ERR_NONICKNAMEGIVEN = 431, ERR_ERRONEUSNICKNAME, ERR_NICKNAMEINUSE,
						ERR_NICKCOLLISION = 436,ERR_NOTONCHANNEL = 442, ERR_USERONCHANNEL,
						ERR_NEEDMOREPARAMS = 461, ERR_ALREADYREGISTRED,
						ERR_CHANOPRIVSNEEDED = 482};
		std::string					prefix;
		std::string					command;
		std::string					arg;
		std::vector<std::string>	args;
		std::string   				text;
		Client&						client;
		Server&						server;
        virtual void	                execute() = 0;
	public:
		virtual void					printMessage(Client* other_client, Channel* channel);
		void							preparationExecute();
		void                            responce(Command::e_resType res, Client* Client, Channel* channel);
        void                            setArgs(std::string args);

		void	                        setCommand(std::string cmd);
		void	                        setPrefix(std::string pref);
		void	                        setText(std::string text);

		const std::string&	            getText() const;
		const std::string&	            getCommand() const;
		const std::string&	            getPrefix() const;
		std::vector<std::string>&		getArgs();
		static void			            printArgs(const std::vector<std::string>& args);

		Command(Server& server,Client& client);
		virtual ~Command();
};

		std::ostream&	operator << (std::ostream& cout, Command& cmd);

#endif 