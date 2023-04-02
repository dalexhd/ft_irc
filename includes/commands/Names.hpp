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

	void send_channel(std::string &name)
	{
		Channel *channel = _server->getChannel(name);
		if (channel == NULL)
			return;
		std::vector<Client *> clients = channel->getClients();
		std::string           users_str = "";
		for (size_t i = 0; i < clients.size(); i++)
		{
			users_str += channel->getClientRoleString(clients[i]);
			if (i == clients.size() - 1)
				users_str += clients[i]->getNick();
			else
				users_str += clients[i]->getNick() + " ";
		}
		_sender->message(
		    RPL_NAMREPLY(_sender->_servername, _sender->_nick, channel->getModeString(), name, users_str));
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
				_sender->message(RPL_ENDOFNAMES(_sender->_servername, _sender->_nick, "#" + _ch_params[i]));
			}
		}
		else
		{
			std::map<std::string, Channel *>::iterator it = _server->_channels.begin();
			for (; it != _server->_channels.end(); it++)
			{
				send_channel(it->second->getName());
				_sender->message(RPL_ENDOFNAMES(_sender->_servername, _sender->_nick,
				                                "#" + it->first)); // TODO: Is this end of channels well implemented?
			}
		}
	}
};
#endif
