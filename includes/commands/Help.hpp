#ifndef HELP_H
#define HELP_H

// Our includes
#include "../Command.hpp"

class Help : public Command
{
  public:
	Help()
	{
		_name = "help";
		_description = "See a guide about each implemented command";
		_usage = "help <command?>";
		_example[0] = "help";
		_example[1] = "help ping";
	}

	void execute()
	{
		std::string                                res = "";
		bool                                       filtering = false;
		std::map<std::string, Command *>::iterator it = _server->_commands.begin();
		std::map<size_t, std::string> params = _message->getParams();

		res = std::string(C_U) + std::string(C_BOLD) + std::string(C_GREEN) + "Commands usage guide" + std::string(C_X) + "\n";

		if (params.size() == 1 && _server->_commands.count(params[0]))
			filtering = true;

		for (; it != _server->_commands.end(); it++)
		{
			if (filtering && it->second->getName() != params[0])
				continue;
			res += std::string("Command: " + it->second->getName() +
			                   "\n\t Description: " + it->second->getDescription() +
			                   "\n\t Usage: " + it->second->getUsage() + "\n\t Example:\n");

			std::map<size_t, std::string> example(it->second->getExample());
			std::map<size_t, std::string>::iterator it_ex = example.begin();
			for (; it_ex != example.end(); it_ex++)
				res += std::string("\t\t" + it_ex->second + "\n");
			res += "\n";
		}
		_sender->message(res.c_str());
	}
};
#endif
