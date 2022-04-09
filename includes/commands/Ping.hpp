#ifndef PING_H
#define PING_H

// Our includes
#include "../Command.hpp"

class Ping : public Command
{
  public:
	Ping()
	{
		_name = "ping";
		_description = "adssa";
		_usage = "ping";
		_example = "ping";
	}

	void execute()
	{
		_sender->message("pong\n");
	}
};
#endif
