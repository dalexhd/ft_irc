#ifndef WHOIS_H
#define WHOIS_H

// Our includes
#include "../Command.hpp"

class Whois : public Command
{
  public:
	Whois()
	{
		_name = "whois";
		_description = "info sobre el usuario usando la mascara";
		_usage = "whois";
		_example[0] = "whois <máscara de nick>[,<máscara de nick>[,...]]";
		_example[1] = "whois guest500";

		// ERR_NOSUCHSERVER FALTA!!!!
		/*
		"<server name> :No such server"

		- Used to indicate the server name given currently
		doesn't exist.
		*/
		// ERR_NONICKNAMEGIVEN
		// ERR_NOSUCHNICK

		// RPL_ENDOFWHOIS
		// RPL_WHOISUSER
		// RPL_WHOISCHANNELS
		// RPL_WHOISIDLE
		// RPL_WHOISOPERATOR
		// RPL_WHOISSERVER
		// RPL_AWAY
		// RPL_WHOISCHANNELS
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		std::vector<std::string>      _cl_params = split(p[0], ",");

		if (_cl_params.size() < 1)
		{
			_sender->message(ERR_NONICKNAMEGIVEN(_sender->_servername));
			return (false);
		}
		for (size_t i = 0; i < _cl_params.size(); i++)
		{
			if (_server->getClient(_cl_params[i]) == NULL)
			{
				_sender->message(ERR_NOSUCHNICK(_sender->_servername, _sender->_nick));
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		std::vector<std::string>      _cl_params = split(p[0], ",");

		for (size_t i = 0; i < _cl_params.size(); i++)
		{
			Client *client = _server->getClient(_cl_params[i]);
			client->message(RPL_WHOISUSER(_sender->_servername, client->_nick,
			                              client->_username, client->_host,
			                              client->_realname));
			std::map<std::string, Channel *> related_channels = _server->getRelatedChannels(client);
			std::map<std::string, Channel *>::iterator it = related_channels.begin();
			std::cout << "servername" << _sender->_servername << std::endl;
			for (; it != related_channels.end(); it++)
			{
				client->message(RPL_WHOISCHANNELS(_sender->_servername, client->_nick,
				                                  it->second->getClientRoleString(client), it->first));
			}
		}
	}
};
#endif
