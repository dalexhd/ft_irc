#ifndef USER_H
#define USER_H

// Our includes
#include "../Command.hpp"

#define _name
#define _decription // Jajaja me he dejado la sesion abierta
class User : public Command
{
  public:
	User()
	{
		//_name = "user";
		_description = "termina una sesión";
		_usage = "user";
		_example[0] = "user <nombre de usuario> <nombre de host> <nombre de "
		              "servidor>: <nombre real>";
		_example[1] = "user telnet";
		_needs_auth = false;
	}

	void execute()
	{
		if (_message->getParams().size() == 0 || _message->getParams().size() > 4)
		{
			_sender->message("Please send valid params! Ex: user <username> "
			                 "<hostname> <servername> : <realname>\n");
		}
		else
		{
			std::string username = _message->getParams()[0];
			std::string realname = _message->getParams()[3];

			if (_sender->_username == username)
			{
				_sender->message("You can't assign the same name yourself!\n");
			}
			else
			{
				std::map<size_t, Client *>::iterator it = _server->_clients.begin();
				for (; it != _server->_clients.end(); it++)
				{
					if (it->second->_username == username)
					{
						it->second->message(ERR_NICKNAMEINUSE(_sender->_servername, username));
						return;
					}
				}
				_sender->setUsername(username);
				_sender->setRealName(realname);
				if (!_sender->isAuthenticated() && !_server->hasPassword())
					_sender->authenticate();
			}
		}
	}
};
#endif
