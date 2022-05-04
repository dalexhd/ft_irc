#ifndef EXIT_H
#define EXIT_H

// Our includes
#include "../Command.hpp"

class Exit : public Command
{
  public:
	Exit()
	{
		_max_params = 0;
		_name = "exit";
		_description = "Disconnect from the IRC server";
		_usage = "exit";
		_example[0] = "exit";
	}

	void execute()
	{
		// First we delete the client pointer, this will execute the client
		// destructor which will close the socket. delete _clients[];
		delete _server->_clients[_sender_index];
		// Then we remove the client from the clients map containers.
		_server->_clients.erase(_server->_clients.begin() + _sender_index);
		// Then we remove the client from the clients _pfds.
		_server->_pfds.erase(_server->_pfds.begin() + _sender_index + 1);
	}
};
#endif
