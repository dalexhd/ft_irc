#ifndef WHO_H
#define WHO_H

// Our includes
#include "../Command.hpp"

class Who : public Command
{
  public:
	Who()
	{
		_name = "who";
		_description = "info sobre el usuario";
		_usage = "who";
		_example[0] = "who [<nombre> [<o>]]";
		_example[1] = "who #uruguay"
	}

	void execute()
	{

	}
};
#endif
