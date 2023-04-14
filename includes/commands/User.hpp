#ifndef USER_H
#define USER_H

// Our includes
#include "../Command.hpp"

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

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();

		if (_sender->getNick() == "")
		{
			return (false);
		}
		if (p.size() < 4)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd()));
			return (false);
		}
		if (_sender->isAuthenticated())
		{
			_sender->message(ERR_ALREADYREGISTRED(_sender->_servername, _sender->_nick));
			return (false);
		}
		return (true);
	}

	void execute()
	{
		std::string username = _message->getParams()[0];
		std::string realname = _message->getParams()[3];
		_sender->setUsername(username);
		_sender->setRealName(realname);
		if(_sender->_is_passLogged)
			_sender->authenticate();
	}
};
#endif
