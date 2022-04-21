#ifndef NICK_H
#define NICK_H

// Our includes
#include "../Command.hpp"

class Nick : public Command
{
  public:
	Nick()
	{
		_name = "Nick";
		_description = "adssa";
		_usage = "Nick";
		_example = "Nick";
	}

	void execute(std::string name)
	{
		_sender->name = name;
	}
};
#endif
