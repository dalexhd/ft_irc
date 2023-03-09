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

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() == 0 || p.size() > 2)
		{
			_sender->message("Wrong command format. Ex: nick <nick>\n");
			return (false);
		}
		std::string                          name = _message->getParams()[0];
		std::map<size_t, Client *>::iterator it = _server->_clients.begin();
		for (; it != _server->_clients.end(); it++)
		{
			if (it->second->_nick == name)
			{
				_sender->message(ERR_NICKNAMEINUSE(_sender->_servername, name));
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, Client *>           clients = _server->_clients;
		std::string                          name = _message->getParams()[0];
		std::map<size_t, Client *>::iterator it = _server->_clients.begin();

		for (; it != _server->_clients.end(); it++)
		{
			if (it->second->_nick == name)
			{
				it->second->message(std::string(":" + _sender->_nick + "!" +
				                                _sender->_username + "@" +
				                                _sender->_servername + " NICK :" + name + "\n")
				                        .c_str());
			}
		}
		_sender->setNick(name);
	}
};
#endif
