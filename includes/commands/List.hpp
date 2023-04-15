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
		_sender->message(RPL_LISTSTART(_sender->_servername, _sender->_nick)); // RPL_LISTSTART (321)
		if (p.size() == 1)
		{
			std::vector<std::string> _ch_params = split(p[0], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				/*bool has_private_mode = std::find(channel->getModes().begin(), channel->getModes().end(), CHANNEL_MODE_PRIVATE) != channel->getModes().end();*/
				//std::cout << "Has private mode " << has_private_mode << "\n";
				if (channel && !(std::find(channel->getModes().begin(), channel->getModes().end(), CHANNEL_MODE_PRIVATE) != channel->getModes().end()))
				{
					_sender->message(RPL_LIST(_sender->_servername, _sender->_nick,
					                          channel->getName(),
					                          itoa(channel->getClients().size()),
					                          channel->getTopic())); // RPL_LIST (322)
				}
			}
		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{
				bool has_private_mode = std::find(channels[i]->getModes().begin(), channels[i]->getModes().end(), CHANNEL_MODE_PRIVATE) != channels[i]->getModes().end();
				if(!has_private_mode)
				{
					_sender->message(RPL_LIST(_sender->_servername, _sender->_nick,
				                          channels[i]->getName(),
				                          itoa(channels[i]->getClients().size()),
				                          channels[i]->getTopic())); // RPL_LIST (322)
				}
			}
		}
		_sender->message(RPL_LISTEND(_sender->_servername, _sender->_nick)); // RPL_LISTEND (323)
	}
};
#endif
