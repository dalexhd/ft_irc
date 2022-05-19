#ifndef ECHO_H
#define ECHO_H

// Our includes
#include "../Command.hpp"

class Echo : public Command
{
  public:
	Echo()
	{
		_name = "echo";
		_description = "Print a set of strings";
		_usage = "echo <string?>";
		_example[0] = "echo";
		_example[1] = "echo hello";
		_example[3] = "echo hello world";
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		std::string                   res = "";

		for (std::map<size_t, std::string>::iterator it = p.begin(); it != p.end(); ++it)
			res += std::string(it->second + " ");

		for (size_t i = 0; i < _server->_clients.size(); i++)
		{
			if (_server->_clients[i] != _sender)
				_server->_clients[i]->message(std::string(res + "\n").c_str());
		}
	}
};
#endif
