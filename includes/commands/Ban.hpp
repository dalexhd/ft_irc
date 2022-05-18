#ifndef BAN_H
#define BAN_H

// Our includes
#include "../Command.hpp"

class Ban : public Command
{
  public:
	Ban()
	{
		_max_params = 1;
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
		else if (p[0] == _sender->_name)
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
		size_t      client_index = _server->getClientIndex(name);

		client->message(
		    std::string("You've been banned by " + _sender->_name + "\n").c_str());
		_sender->message(
		    std::string("Client " + client->_name + " has been banned!\n").c_str());

		// First we delete the client pointer, this will execute the client
		// destructor which will close the socket. delete _clients[];
		delete client;
		// Then we remove the client from the clients map containers.
		_server->_clients.erase(_server->_clients.begin() + client_index);
		// Then we remove the client from the clients _pfds.
		_server->_pfds.erase(_server->_pfds.begin() + client_index + 1);
	}
};
#endif
