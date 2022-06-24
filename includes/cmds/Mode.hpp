#ifndef MODE_H
#define MODE_H

// Our includes
#include "../Command.hpp"

class Mode : public Command
{
  public:
	Mode()
	{
		_name = "mode";
		_description = "expulsar del servidor";
		_usage = "mode";
		_example[0] = "mode canal> {[+|-]|o|p|s|i|t|n|b|v} [<límite>] "
		              "[<usuario>] [<máscara de ban>]";
		_example[1] = "mode <nick> {[+|-]|i|w|s|o}";
		_is_ope = true;
	}

	bool validate(void)
	{
		std::map<size_t, std::string> target = _message->getParams();

		if (target.size() < 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, _name)); // ERR_NEEDMOREPARAMS (461)
			return (false);
		}
		else if (target[0].at(0) == '#')
		{
			if (!_server->getChannel(target[0]))
			{
				_sender->message(
				    std::string("Channel" + target[0] + "doesn't exist\n").c_str());
				return (false);
			}
		}
		else
		{
			if (_server->getClient(target[0]) == NULL)
			{
				std::cout << _sender->_nick << target[0] << std::endl;
				_sender->message(ERR_NOSUCHNICK(_sender->_servername, target[0]));
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
	}
};
#endif
