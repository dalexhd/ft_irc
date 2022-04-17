#ifndef INFO_H
#define INFO_H

// Our includes
#include "../Command.hpp"

std::string itoa(int a)
{
	std::string ss = ""; // create empty string
	while (a)
	{
		int x = a % 10;
		a /= 10;
		char i = '0';
		i = i + x;
		ss = i + ss; // append new character at the front of the string!
	}
	return ss;
}

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
		size_t connectedClients = _server->_clients.size();
		_sender->message(
		    std::string("Connected clients: " + itoa(connectedClients) + "\n").c_str());
	}
};
#endif
