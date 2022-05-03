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
	}

	void execute()
	{

	}
};
#endif
