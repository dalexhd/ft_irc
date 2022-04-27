#ifndef CLOSE_H
#define CLOSE_H

// Our includes
#include "../Command.hpp"

class Close : public Command
{
  public:
	Close()
	{
		_name = "close";
		_description = "termina una sesión";
		_usage = "close";
		_example[0] = "close";
	}

	void execute()
	{

	}
};
#endif
