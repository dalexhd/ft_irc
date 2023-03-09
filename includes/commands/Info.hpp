#ifndef INFO_H
#define INFO_H

// Our includes
#include "../Command.hpp"

class Info : public Command
{
  public:
	Info()
	{
		_name = "info";
		_description = "Get info about the current IRC server";
		_usage = "info";
		_example[0] = "info";
	}

	void execute()
	{
		_sender->message(
		    std::string("Connected clients: " + itoa(_server->_clients.size()) + "\n" +
		                "Name: " + _sender->_nick + "\n")
		        .c_str());
		for (size_t i = 0; i < _server->_clients.size(); i++)
		{
			if (_server->_clients[i] != _sender)
			{
				_sender->message(
				    std::string("Client " + _server->_clients[i]->_nick + "\n").c_str());
			}
		}
	}
};
#endif
