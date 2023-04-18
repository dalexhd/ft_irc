#ifndef LIST_H
#define LIST_H

// Our includes
#include "../Command.hpp"

class List : public Command
{
  public:
	List()
	{
		_name = "list";
		_description = "listar informacion del canal";
		_usage = "list [<canales>]";
		_example[0] = "list";
		_example[1] = "list #hola";
		_example[2] = "list #hola,#chau";
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		_sender->message(RPL_LISTSTART(_sender->_servername, _sender->getNick()));
		if (p.size() == 1)
		{
			std::vector<std::string> _ch_params = split(p[0], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					_sender->message(RPL_LIST(
					    _sender->_servername, _sender->getNick(), channel->getName(),
					    itoa(channel->getClients().size()), channel->getTopic()));
				}
			}
		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{
				if (!channels[i]->hasMode(CHANNEL_MODE_SECRET) || channels[i]->joined(_sender))
				{
					_sender->message(RPL_LIST(
					    _sender->_servername, _sender->getNick(), channels[i]->getName(),
					    itoa(channels[i]->getClients().size()), channels[i]->getTopic()));
				}
			}
		}
		_sender->message(RPL_LISTEND(_sender->_servername, _sender->getNick()));
	}
};
#endif
