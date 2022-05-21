#ifndef KICK_H
#define KICK_H

// Our includes
#include "../Command.hpp"

class Kick : public Command
{
  public:
	Kick()
	{
		_name = "kick";
		_description = "expulsar del servidor";
		_usage = "kick";
		_example[0] = "kick <canal> <usuario> :[<comentario>]";
		_is_ope = true;

		/*
		REPLIES
		 "<client> <command> :Not enough parameters"
		 "<client> <channel> :No such channel"
		 "<client> <channel> :You're not channel operator"
		 "<client> <nick> <channel> :They aren't on that channel"
		 "<client> <channel> :You're not on that channel"
		*/
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() > 1)
		{
			_sender->message("Wrong command format. Ex: list "
			                 "[<canal>{,<canal>}]\n");
			return (false);
		}
		return (true);

	}

	void execute()
	{

	}
};
#endif
