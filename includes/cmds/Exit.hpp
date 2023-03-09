#ifndef EXIT_H
#define EXIT_H

// Our includes
#include "../Command.hpp"

class Exit : public Command
{
  public:
	Exit()
	{
		_name = "exit";
		_description = "Disconnect from the IRC server";
		_usage = "exit";
		_example[0] = "exit";
	}

	void execute()
	{
		this->_server->deleteClient(this->_sender->_fd);
	}
};
#endif
