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
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1)
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
					_sender->message(
					    ERR_BADCHANMASK(_sender->_servername, _sender->_nick)); // ERR_BADCHANMASK (476)
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
						_sender->message(
						    ERR_NOTONCHANNEL(_sender->_servername, _sender->_nick, _ch_params[i]));
						return (false);
					}
				}
				else
				{
					_sender->message(
					    ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_nick, _ch_params[i]));
					return (false);
				}
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();

		std::vector<std::string> _ch_params = split(p[0], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			Channel *channel = _server->getChannel(_ch_params[i]);

			std::vector<Client *> related_channels_clients = channel->getClients();
			for (size_t j = 0; j < related_channels_clients.size(); j++)
			{
				related_channels_clients[j]->message(
				    ":" + _sender->_nick + "!" + _sender->_username + "@" +
				    _sender->_host + " PART #" + channel->getName() + " :" + p[1] + "\n");
			}
			channel->removeClientFromChannel(_sender);
		}
	}
};
#endif
