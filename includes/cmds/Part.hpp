#ifndef PART_H
#define PART_H

// Our includes
#include "../Command.hpp"

class Part : public Command
{
  public:
	Part()
	{
		_name = "part";
		_description = "salir de un canal";
		_usage = "part";
		_example[0] = "part <canal>{,<canal>}";
		_example[1] = "part :#uruguay";
	}

	void execute()
	{

	}
};
#endif
