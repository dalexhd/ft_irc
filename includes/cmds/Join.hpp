#ifndef JOIN_H
#define JOIN_H

// Our includes
#include "../Command.hpp"

class Join : public Command
{
  public:
	Join()
	{
		_name = "join";
		_description = "une al usuario al canal";
		_usage = "join";
		_example[0] = "join <canal>{,<canal>} [<clave>{,<clave>}]";
		_example[1] = "join #canal1,#canal2 clave1,clave2"
	}

	void execute()
	{

	}
};
#endif
