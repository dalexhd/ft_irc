#ifndef PRIVMSG_H
#define PRIVMSG_H

// Our includes
#include "../Command.hpp"

class PrivMsg : public Command
{
  public:
	PrivMsg()
	{
		_name = "privmsg";
		_description = "Send message to the user";
		_usage = "privmsg <nickname> <message>";
		_example[0] = "privmsg alfred hello";
		_example[1] = "privmsg alfred hello world";
	}

	void execute()
	{
		if (_message->getParams().size() < 2)
		{
			_sender->message("Please send valid params! Ex: privmsg <nickname> <message>\n");
		} else {
			std::map<size_t, std::string> p = _message->getParams();

			std::string                   res = "";
			for (std::map<size_t, std::string>::iterator it = p.begin(); it != p.end(); ++it)
				if (it->first != 0)
					res += std::string(it->second + " ");
			if (_name == _sender->_name)
			{
				_sender->message("You can't send a message  yourself!\n");
			} else {
				for (size_t i = 0; i < _server->_clients.size(); i++)
				{
					if (_server->_clients[i]->_name == p[0])
					{
						_server->_clients[i]->message(std::string(_sender->_name + " sent you a message: " +  res + "\n").c_str());
						return ;
					}
				}
				_sender->message("Client not found!\n");
			}
		}
	}
};
#endif
