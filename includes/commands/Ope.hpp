#ifndef OPE_H
#define OPE_H

// Our includes
#include "../Command.hpp"

class Ope : public Command
{
  public:
	Ope()
	{
		_name = "ope";
		_description = "Assign operator role yourself";
		_usage = "ope";
		_example[0] = "ope";
		_is_ope = false;
	}

	void execute()
	{
		_sender->_is_ope = !_sender->_is_ope;
		_sender->message(_sender->_is_ope ? "Now you're operator!\n" : "Ope role removed!\n");
	}
};
#endif
