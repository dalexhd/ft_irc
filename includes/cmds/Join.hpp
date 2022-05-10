#ifndef JOIN_H
#define JOIN_H

// Our includes
#include "../Command.hpp"

class Join : public Command
{
  public:
	Join()
	{
		_name = "join";
		_description = "une al usuario al canal si existe o genera un nuevo "
		               "canal";
		_usage = "join <canal>{,<canal>} [<clave>{,<clave>}]";
		_example[0] = "join #canal1 clave1";
		_example[1] = "join #canal1,#canal2 clave1,clave2";
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1 || p.size() > 2)
		{
			_sender->message("Wrong command format. Ex: join #canal1,#canal2 "
			                 "clave1,clave2\n");
			return (false);
		}
		else
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			std::vector<std::string> _pw_params = split(p[1], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				if (_ch_params[i][0] != '#')
				{
					_sender->message("Wrong command format. Ex: join "
					                 "#canal1,#canal2 "
					                 "clave1,clave2\n");
					return (false);
				}
			}
			if (_pw_params.size() > 0 && _pw_params.size() != _ch_params.size())
			{
				_sender->message("Wrong command format. Ex: join "
				                 "#canal1,#canal2 "
				                 "clave1,clave2\n");
				return (false);
			}
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					if (channel->joined(_sender))
					{
						_sender->message("You're already member of that "
						                 "channel!\n");
						return (false);
					}
				}
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();

		std::vector<std::string> _ch_params = split(p[0], ",");
		std::vector<std::string> _pw_params = split(p[1], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			Channel *channel = _server->getChannel(_ch_params[i]);
			if (channel)
			{
				if (_pw_params.size() > 0)
				{
					if (channel->getPassword() == _pw_params[i])
					{
						channel->_normal_clients.push_back(_sender);
						_sender->message(std::string("Client " + _sender->_name + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
					}
					else
						_sender->message("Wrong password\n");
				}
				else
				{
					if (channel->getPassword() != "")
						_sender->message("Wrong password\n");
					else
					{
						channel->_normal_clients.push_back(_sender);
						_sender->message(std::string("Client " + _sender->_name + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
					}
				}
			}
			else
			{
				Channel *channel;
				if (_pw_params.size() > 0)
				{
					channel = _server->createChannel(_ch_params[i], _pw_params[i]);
					_sender->message(std::string("Channel " + _ch_params[i] + " with password \"" + _pw_params[i] + "\" created\n")
					                     .c_str());
				}
				else
				{
					std::string pwd = std::string("");
					channel = _server->createChannel(_ch_params[i], pwd);
					_sender->message(std::string("Channel " + _ch_params[i] + " with empty password created\n")
					                     .c_str());
				}
				channel->setCreator(_sender);
				_sender->message(std::string("Client " + _sender->_name + " joined channel " + _ch_params[i] + "\n")
				                     .c_str());
				channel->_ope_clients.push_back(_sender);
			}
		}
	}
};
#endif
