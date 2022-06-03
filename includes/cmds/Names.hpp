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
				_sender->message(ERR_BADCHANMASK(_sender->_servername, _sender->_nick)); // ERR_BADCHANMASK (476)
				return (false);
			}
		}
		return (true);
	}

	void send_channel(std::string &name)
	{
		Channel *             channel = _server->getChannel(name);
		std::vector<Client *> clients = channel->getClients();
		std::string           users_str = "";
		for (size_t i = 0; i < clients.size(); i++)
		{
			if (i == clients.size() - 1)
				users_str += clients[i]->getNick();
			else
				users_str += clients[i]->getNick() + " ";
		}
		_sender->message(RPL_NAMREPLY(_sender->_servername, _sender->_nick, name, users_str));
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() == 1)
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				send_channel(_ch_params[i]);
				_sender->message(RPL_ENDOFNAMES(_sender->_servername, _sender->_nick, _ch_params[i]));
			}
		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{
				send_channel(channels[i]->getName());
				_sender->message(RPL_ENDOFNAMES(_sender->_servername, _sender->_nick,
				                                channels[i]->getName())); // TODO: Is this end of channels well implemented?
			}
		}
	}
};
#endif
