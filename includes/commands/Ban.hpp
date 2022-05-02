#ifndef BAN_H
#define BAN_H

// Our includes
#include "../Command.hpp"

class Ban : public Command
{
  public:
	Ban()
	{
		_min_params = 0;
		_max_params = 1;
		_name = "ban";
		_description = "Just ban a client";
		_usage = "ban <client_nickname>";
		_example[0] = "ban alfred";
		_is_ope = true;
	}

	void execute()
	{
		if (_message->getParams().size() == 0 || _message->getParams().size() > 2)
		{
			_sender->message("Please send valid params! Ex: ban <client_nickname>\n");
		} else {
			std::string name = _message->getParams()[0];
			if (name == _sender->_name)
			{
				_sender->message(std::string("You cant ban yourself\n").c_str());
				return ;
			}
			for (size_t i = 0; i < _server->_clients.size(); i++)
			{
				if (_server->_clients[i]->_name == name)
				{
					_server->_clients[i]->message(std::string("You've been banned by " + _sender->_name + "\n").c_str());
					_sender->message(std::string("Client " + _server->_clients[i]->_name + " has been banned!\n").c_str());

					// First we delete the client pointer, this will execute the client
					// destructor which will close the socket. delete _clients[];
					delete _server->_clients[i];
					// Then we remove the client from the clients map containers.
					_server->_clients.erase(_server->_clients.begin() + i);
					// Then we remove the client from the clients _pfds.
					_server->_pfds.erase(_server->_pfds.begin() + i + 1);

					return ;
				}
			}
			_sender->message("Client not found!\n");
		}
	}
};
#endif
