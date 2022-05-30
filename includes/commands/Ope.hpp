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
			Client *client;
			if ((client = _server->getClient(_message->getParams()[0])))
			{
				client->_is_ope = !client->_is_ope;
				_sender->message(std::string(client->_is_ope ?
				                                 "Now the client " + client->_nick + " is an operator!\n" :
				                                 "Now the client " + client->_nick + " isn't an operator!\n")
				                     .c_str());
				client->message(
				    std::string(client->_is_ope ? "Now you're an operator!\n" : "Now you aren't an operator!\n")
				        .c_str());
				return;
			}
			else
				_sender->message("Client not found!\n");
		}
	}
};
#endif
