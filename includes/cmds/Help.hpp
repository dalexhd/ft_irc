#ifndef HELP_H
#define HELP_H

// Our includes
#include "../Command.hpp"

class Help : public Command
{
  public:
	Help()
	{
		_name = "?\help";
		_description = "muestra la ayuda";
		_usage = "?\help";
		_example[0] = "?\help";
	}

	void execute()
	{

	}
};
#endif
