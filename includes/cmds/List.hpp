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
		_example[0] = "list #hola";
		_example[0] = "list #hola,#chau";
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() > 1)
		{
			_sender->message("Wrong command format. Ex: list "
			                 "[<canal>{,<canal>}]\n");
			return (false);
		}
		return (true);
	}

	void execute()
	{
		/*std::cout << "NUM OF CHANNELS " << _server->getChannels().size() << std::endl;
		if(_server->getChannels().size() == 0)
			std::cout << "NO HAY CANALES " << std::endl;*/
		if (!validate())
			return;
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() == 1)
		{
			std::vector<std::string> _ch_params = split(p[0], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					_sender->message(std::string("Channel: " + channel->getName() + " " + (channel->joined(_sender) ? "(Joined)" : "") + "\n")
										.c_str());
				}
				else
					_sender->message(std::string("Channel: " + _ch_params[i] + " not found\n")
										.c_str());
			}

		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{
				_sender->message(std::string("Channel: " + channels[i]->getName() +
				                             " " + (channels[i]->joined(_sender) ? "(Joined)" : "") + "\n")
				                     .c_str());
			}
		}
	}
};
#endif
