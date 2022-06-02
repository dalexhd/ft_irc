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
				_sender->message(ERR_BADCHANMASK(_sender->_servername, _sender->_nick)); // ERR_BADCHANMASK (476)
				return (false);
			}
		}
		return (true);
	}

	void send_channel(std::string &name)
	{
		Channel *                channel = _server->getChannel(name);
		std::vector<std::string> users;
		std::string              users_str = "";
		for (size_t i = 0; i < channel->_normal_clients.size(); i++)
			users.push_back(channel->_normal_clients[i]->_nick);
		for (size_t i = 0; i < channel->_ope_clients.size(); i++)
			users.push_back(channel->_ope_clients[i]->_nick);
		for (size_t i = 0; i < users.size(); i++)
		{
			if (i == users.size() - 1)
				users_str += users[i];
			else
				users_str += users[i] + " ";
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
				send_channel(channels[i]->getName());
			_sender->message(std::string(_sender->_nick + " " + "*" + " : End of /NAMES list\n")
			                     .c_str());
		}
	}
};
#endif
