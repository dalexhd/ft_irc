#ifndef NOTICE_H
#define NOTICE_H

// Our includes
#include "../Command.hpp"

class Notice : public Command
{
  public:
	Notice()
	{
		_name = "notice";
		_description = "Send message to a set of users/channels";
		_usage = "notice <receptor|channel>{,<receptor|channel>} :<texto>";
		_example[0] = "notice alfred hello";
		_example[1] = "notice alfred,manolo hello";
		_example[2] = "notice #uruguay :hola, buenas tardes";
		_example[3] = "notice #uruguay,#peru :hola, buenas tardes";
	}

	/*
	The NOTICE message is used similarly to PRIVMSG.  The difference
   between NOTICE and PRIVMSG is that automatic replies must never be
   sent in response to a NOTICE message.
   */

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 2)
		{
			return (false);
		}
		else if (p[0].at(0) == '#')
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			for (size_t i = 1; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel == NULL || channel->joined(_sender) == false ||
				    (channel->isModerated() && channel->isOpe(_sender) == false))
				{
					return (false);
				}
			}
			return (true);
		}
		else
		{
			std::vector<std::string> _cl_params = split(p[0], ",");
			for (size_t i = 0; i < _cl_params.size(); i++)
			{
				if (_server->getClient(_cl_params[i]) == NULL ||
				    _cl_params[i] == _sender->_nick)
				{
					return (false);
				}
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		std::string                   msg = std::string(p[1] + "\n").c_str();
		if (p[0].at(0) == '#')
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				std::string           name = _ch_params[i].substr(1).c_str();
				Channel *             channel = _server->getChannel(name);
				std::vector<Client *> clients = channel->getClients();
				for (size_t j = 0; j < clients.size(); j++)
				{
					if (clients[j] != _sender)
					{
						clients[j]->message(std::string(":" + _sender->_nick + " NOTICE #" +
						                                channel->getName() + " :" + msg + "\n")
						                        .c_str());
					}
				}
			}
		}
		else
		{
			std::vector<std::string> _cl_params = split(p[0], ",");
			for (size_t i = 0; i < _cl_params.size(); i++)
			{
				Client *client = _server->getClient(_cl_params[i]);
				client->message(
				    std::string(":" + _sender->_nick + " NOTICE " + client->getNick() + " :" + msg + "\n")
				        .c_str());
			}
		}
	}
};

#endif
