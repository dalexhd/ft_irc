#ifndef BAN_H
#define BAN_H

// Our includes
#include "../Command.hpp"

class Ban : public Command
{
  public:
	Ban()
	{
		_name = "ban";
		_description = "Just ban a client";
		_usage = "ban <client_nickname>";
		_example[0] = "ban alfred";
		_is_ope = true;
	}

	void execute()
	{
		_sender->message("ban\n");
	}
};
#endif
