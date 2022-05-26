#ifndef NICK_H
#define NICK_H

// Our includes
#include "../Command.hpp"

class Nick : public Command
{
  public:
	Nick()
	{
		_name = "nick";
		_description = "";
		_usage = "nick";
		_example[0] = "nick <nick>";
		_example[1] = "nick <nuevo_nick>";
		_needs_auth = false;
	}

	void execute()
	{
		if (_message->getParams().size() == 0 || _message->getParams().size() > 2)
		{
			_sender->message("Please send valid params! Ex: name <nickname>\n");
		}
		else
		{
			std::string name = _message->getParams()[0];
			if (_sender->_nick == name)
			{
				_sender->message("You can't assign the same name yourself!\n");
			}
			else
			{
				for (size_t i = 0; i < _server->_clients.size(); i++)
				{
					if (_server->_clients[i]->_nick == name)
					{
						_sender->message(ERR_NICKNAMEINUSE(_sender->_servername, name));
						return;
					}
				}
				_sender->setNick(name);
			}
		}
	}
};
#endif
