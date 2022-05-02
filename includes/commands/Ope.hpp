#ifndef OPE_H
#define OPE_H

// Our includes
#include "../Command.hpp"

class Ope : public Command
{
  public:
	Ope()
	{
		_min_params = 0;
		_max_params = 1;
		_name = "ope";
		_description = "Assign operator role yourself or a client";
		_usage = "ope <nikname?>";
		_example[0] = "ope";
		_example[1] = "ope alfred";
		_is_ope = false;
	}

	void execute()
	{
		if (_message->getParams().size() == 0)
		{
			_sender->_is_ope = !_sender->_is_ope;
			_sender->message(_sender->_is_ope ? "Now you're operator!\n" : "Ope role removed!\n");
		}
		else if (_message->getParams().size() > 1)
		{
			_sender->message("Please send valid params! Ex: ope <nikname?>\n");
		}
		else
		{
			for (size_t i = 0; i < _server->_clients.size(); i++)
			{
				if (_server->_clients[i]->_name == _message->getParams()[0])
				{
					_server->_clients[i]->_is_ope = !_server->_clients[i]->_is_ope;
					_sender->message(std::string(_server->_clients[i]->_is_ope ? "Now the client " + _server->_clients[i]->_name + " is an operator!\n" :
						"Now the client " + _server->_clients[i]->_name + " isn't an operator!\n").c_str());
					_server->_clients[i]->message(std::string(_server->_clients[i]->_is_ope ? "Now you're an operator!\n" :
						"Now you aren't an operator!\n").c_str());
					return ;
				}
			}
			_sender->message("Client not found!\n");
		}
	}
};
#endif
