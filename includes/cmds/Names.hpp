#ifndef NAMES_H
#define NAMES_H

// Our includes
#include "../Command.hpp"

class Names : public Command
{
  public:
	Names()
	{
		_name = "Names";
		_description = "Mostrar los usuarios pertenecianentes a los canales";
		_usage = "names [<canales>]";
		_example[0] = "names";
		_example[0] = "names #hola";
		_example[0] = "names #hola,#chau";

		// RPL_NAMREPLY (353) k
		// RPL_ENDOFNAMES (366) k
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
		std::vector<std::string> _ch_params = split(p[0], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			if (_ch_params[i][0] != '#')
			{
				ERR_BADCHANMASK(_sender->_servername,_sender->_name); // ERR_BADCHANMASK (476)
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
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
					// "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
					_sender->message(std::string(_sender->_name + " " + "=" + " " + channel->getName() + ": ").c_str());
					for (size_t j = 0; j < channel->_normal_clients.size(); j++)
						_sender->message(std::string(channel->_normal_clients[j]->_name + " ").c_str());
					for (size_t j = 0; j < channel->_ope_clients.size(); j++)
						_sender->message(std::string(channel->_ope_clients[j]->_name + " ").c_str());
					_sender->message(std::string("\n").c_str());
				}
				else
					_sender->message(std::string("Channel: " + _ch_params[i] + " not found\n")
										.c_str());
				// "<client> <channel> :End of /NAMES list"
				_sender->message(std::string(_sender->_name + " " + channel->getName() + " : End of /NAMES list\n").c_str());
			}
		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{
				// "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
				_sender->message(std::string(_sender->_name + " " + "=" + " " + channels[i]->getName() + ": ").c_str());
					for (size_t j = 0; j < channels[i]->_normal_clients.size(); j++)
						_sender->message(std::string(channels[i]->_normal_clients[j]->_name + " ").c_str());
					for (size_t j = 0; j < channels[i]->_ope_clients.size(); j++)
						_sender->message(std::string(channels[i]->_ope_clients[j]->_name + " ").c_str());
					_sender->message(std::string("\n").c_str());
			}
			_sender->message(std::string(_sender->_name + " " + "*" + " : End of /NAMES list\n").c_str());
		}
	}
};
#endif
