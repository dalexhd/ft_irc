#ifndef NICK_H
#define NICK_H

// Our includes
#include "../Command.hpp"

class Nick : public Command
{
  public:
	Nick()
	{
		_name = "nick";
		_description = "";
		_usage = "nick";
		_example[0] = "nick <nick>";
		_example[1] = "nick <nuevo_nick>";
	}

	void execute()
	{
	}
};
#endif
