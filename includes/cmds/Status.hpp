#ifndef STATUS_H
#define STATUS_H

// Our includes
#include "../Command.hpp"

class Status : public Command
{
  public:
	Status()
	{
		_name = "status";
		_description = "Muestra el estado actual de Telnet";
		_usage = "status";
		_example[0] = "status";
	}

	void execute()
	{
	}
};
#endif
