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
	// ERR_NONICKNAMEGIVEN k
	// ERR_ERRONEUSNICKNAME is alphanumeric
	// ERR_NICKNAMEINUSE k
	// ERR_NICKCOLLISION nick existente en otro server conectado al server. Error exclusivo de redes con varios servidores.

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() == 0 || p.size() > 2)
		{
			if (p.size() < 1)
				_sender->message(ERR_NONICKNAMEGIVEN(_sender->_servername));
			else
				_sender->message("Wrong command format. Ex: nick <nick>\n"); // ESTO EXISTE EN EL PROTOCOLO??????????
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
		/*
		A through to Z. (Lowercase and uppercase.)
		0 through to 9.
		`|^_-{}[] and \
		And a name cannot start with a number or hyphen.
		*/
		if (std::count_if(name.begin(), name.end(), ::isalnum) == name.length())
		{
			_sender->message(ERR_ERRONEUSNICKNAME(_sender->_servername, name)); // NO ESTA DEFINIDO EL ERROR
			return (false);
		}

		return (true);
	}

	void execute() // doesnt work fine
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
