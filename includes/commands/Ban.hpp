#ifndef BAN_H
#define BAN_H

// Our includes
#include "../Command.hpp"

class Ban : public Command
{
  public:
	Ban()
	{
		_name = "ban";
		_description = "Just ban a client";
		_usage = "ban <client_nickname>";
		_example[0] = "ban alfred";
		_is_ope = true;
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();

		if (p.size() == 0 || p.size() > 2)
		{
			_sender->message("Please send valid params! Ex: ban "
			                 "<client_nickname>\n");
			return (false);
		}
		else if (p[0] == _sender->_nick)
		{
			_sender->message(std::string("You cant ban yourself\n").c_str());
			return (false);
		}
		else if (!_server->getClient(p[0]))
		{
			_sender->message("Client not found!\n");
			return (false);
		}
		return (true);
	}

	void execute()
	{
		std::string name = _message->getParams()[0];
		Client *    client = _server->getClient(name);

		client->message(
		    std::string("You've been banned by " + _sender->_nick + "\n").c_str());
		_sender->message(
		    std::string("Client " + client->_nick + " has been banned!\n").c_str());

		this->_server->deleteClient(client->_fd);
	}
};
#endif
