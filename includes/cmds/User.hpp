#ifndef USER_H
#define USER_H

// Our includes
#include "../Command.hpp"

class User : public Command
{
  public:
	User()
	{
		_name = "user";
		_description = "termina una sesión";
		_usage = "user";
		_example[0] = "user <nombre de usuario> <nombre de host> <nombre de "
		              "servidor>: <nombre real>";
		_example[1] = "user telnet";
	}

	void execute()
	{
	}
};
#endif
