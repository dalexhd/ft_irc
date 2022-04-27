#ifndef LIST_H
#define LIST_H

// Our includes
#include "../Command.hpp"

class List : public Command
{
  public:
	List()
	{
		_name = "list";
		_description = "salir del servidor con mensaje de salida";
		_usage = "list";
		_example[0] = "list [<canal>{,<canal>}";
	}

	void execute()
	{

	}
};
#endif
