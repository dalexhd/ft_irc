#ifndef PART_H
#define PART_H

// Our includes
#include "../Command.hpp"

class Part : public Command
{
  public:
	Part()
	{
		_name = "part";
		_description = "salir de un canal";
		_usage = "part";
		_example[0] = "part <canal>{,<canal>}";
		_example[1] = "part :#uruguay";

		// ERR_NEEDMOREPARAMS (461) k
		// ERR_NOSUCHCHANNEL (403) k
		// ERR_NOTONCHANNEL (442) k
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1 || p.size() > 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd()));
			return (false);
		}
		else
		{
			std::vector<std::string> _ch_params = split(p[0], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				if (_ch_params[i][0] != '#')
				{
					_sender->message(ERR_BADCHANMASK(_sender->_servername,_sender->_nick)); // ERR_BADCHANMASK (476)
					return (false);
				}
			}
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					if (!channel->joined(_sender))
					{
						_sender->message(ERR_NOTONCHANNEL(_sender->_servername, _sender->_nick, _ch_params[i]));
						return (false);
					}
				}
				else
				{
					_sender->message(ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_nick, _ch_params[i]));
					return (false);
				}
			}
		}
		return (true);
	}

	void execute()
	{
		if (!validate())
			return;
		std::map<size_t, std::string> p = _message->getParams();

		std::vector<std::string> _ch_params = split(p[0], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			Channel *channel = _server->getChannel(_ch_params[i]);
			if (channel)
			{
				for (size_t j = 0; j < channel->_normal_clients.size(); j++)
				{
					if (channel->_normal_clients[j]->_nick == _sender->_nick)
						channel->_normal_clients.erase(channel->_normal_clients.begin() + j);

				}
				for (size_t j = 0; j < channel->_ope_clients.size(); j++)
				{
					if (channel->_ope_clients[j]->_nick == _sender->_nick)
						channel->_ope_clients.erase(channel->_ope_clients.begin() + j);
				}
				_sender->message(std::string("Client " + _sender->_nick + " parted channel " + _ch_params[i] + "\n")
				                     .c_str());
			}
		}
	}
};
#endif
