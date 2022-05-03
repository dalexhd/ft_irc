#ifndef INVITE_H
#define INVITE_H

// Our includes
#include "../Command.hpp"

class Invite : public Command
{
  public:
	Invite()
	{
		_name = "invite";
		_description = "invita usuario a canal";
		_usage = "invite";
		_example[0] = "invite <nick> <canal>";
//		_is_opec = true;
	}

	void execute()
	{

	}
};
#endif
