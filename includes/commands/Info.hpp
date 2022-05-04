#ifndef INFO_H
#define INFO_H

// Our includes
#include "../Command.hpp"

class Info : public Command
{
  public:
	Info()
	{
		_max_params = 1;
		_name = "info";
		_description = "Get info about the current IRC server";
		_usage = "info";
		_example[0] = "info";
	}

	void execute()
	{
		size_t connectedClients = _server->_clients.size();
		_sender->message(std::string("Connected clients: " + itoa(connectedClients) + "\n" + "Name: " + _sender->_name + "\n").c_str());
		for (size_t i = 0; i < _server->_clients.size(); i++)
		{
			if (_server->_clients[i] != _sender)
			{
				_sender->message(std::string("Client " + _server->_clients[i]->_name + "\n").c_str());
			}
		}
	}
};
#endif
