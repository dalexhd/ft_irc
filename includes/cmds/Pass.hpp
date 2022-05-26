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

	void execute()
	{
		if (!_sender->isAuthenticated() && _sender->canPerformPassCommand())
		{
			if (_message->getParams()[0] == _server->getPassword())
				_sender->authenticate();
			else
			{
				_sender->message(
				    ERR_PASSWDMISMATCH(_sender->_servername, _sender->_nick));
				_sender->loginFail();
			}
		}
	}
};
#endif
