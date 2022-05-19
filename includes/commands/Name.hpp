#ifndef NAME_H
#define NAME_H

// Our includes
#include "../Command.hpp"

class Name : public Command
{
  public:
	Name()
	{
		_name = "name";
		_description = "Set your nickname";
		_usage = "name <nickname>";
		_example[0] = "nick alfred";
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
			if (_sender->_name == name)
			{
				_sender->message("You can't assign the same name yourself!\n");
			}
			else
			{
				for (size_t i = 0; i < _server->_clients.size(); i++)
				{
					if (_server->_clients[i]->_name == name)
					{
						_sender->message("That name is already taken!\n");
						return;
					}
				}
				_sender->setName(name);
			}
		}
	}
};
#endif
