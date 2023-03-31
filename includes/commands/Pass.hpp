#ifndef PASS_H
#define PASS_H

// Our includes
#include "../Command.hpp"

class Pass : public Command
{
  public:
	Pass()
	{
		_name = "pass";
		_description = "inicia sesion con el pass";
		_usage = "pass <password>";
		_example[0] = "pass 1234";
		_needs_auth = false;
	}

	// ERR_NEEDMOREPARAMS comprobar tras arreglar nick

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 2)
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
		_sender->authenticate();
	}
};
#endif
