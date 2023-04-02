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

		if (p.size() == 0)
		{
			_sender->message(ERR_NONICKNAMEGIVEN(_sender->_servername));
			return (false);
		}
		else if (p.size() > 1)
		{
			_sender->message(ERR_ERRONEUSNICKNAME(_sender->_servername, p[0]));
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
		/*		A name can only contain the following characters:

		        A through to Z. (Lowercase and uppercase.)
		        0 through to 9.
		        `|^_-{}[] and \

		        And a name cannot start with a number or hyphen.
		*/
		if (std::count_if(name.begin(), name.end(), ::isalnum) != static_cast<long>(name.length())) // MAAAAL
		{
			_sender->message(ERR_ERRONEUSNICKNAME(_sender->_servername, name));
			return (false);
		}
		return (true);
	}

	void execute() // doesnt work fine
	{
		std::map<size_t, Client *> clients = _server->_clients;
		std::string                name = _message->getParams()[0];

		std::vector<Client *> related_clients = _server->getRelatedClients(_sender);
		for (size_t i = 0; i < related_clients.size(); i++)
		{
			related_clients[i]->message(std::string(":" + _sender->_nick + "!" +
			                                        _sender->_username + "@" +
			                                        _sender->_servername + " NICK :" + name + "\n")
			                                .c_str());
		}
		_sender->message(std::string(":" + _sender->_nick + "!" +
			                        _sender->_username + "@" +
			                        _sender->_servername + " NICK :" + name + "\n")
									.c_str());
		_sender->setNick(name);
	}
};
#endif
