#ifndef PRIVMSG_H
#define PRIVMSG_H

// Our includes
#include "../Command.hpp"

class PrivMsg : public Command
{
  public:
	PrivMsg()
	{
		_name = "privmsg";
		_description = "Send message to a set of users/channels";
		_usage = "privmsg <receptor|channel>{,<receptor|channel>} :<texto>";
		_example[0] = "privmsg alfred hello";
		_example[1] = "privmsg alfred,manolo hello";
		_example[2] = "privmsg #uruguay :hola, buenas tardes";
		_example[3] = "privmsg #uruguay,#peru :hola, buenas tardes";
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd()));
			return (false);
		}
		else if (p[0].at(0) == '#')
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel == NULL)
				{
					_sender->message(
					    ERR_NOSUCHCHANNEL(_sender->_servername, _sender->getNick(), _ch_params[i]));
					return (false);
				}
				else if ((channel == NULL ||
				          (channel->joined(_sender) == false && !channel->hasMode(CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE)) ||
				          (channel->isModerated() && channel->isOpe(_sender) == false)))

				{
					_sender->message(ERR_CANNOTSENDTOCHAN(
					    _sender->_servername, _sender->_nick, channel->getName()));
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
				if (_server->getClient(_cl_params[i]) == NULL)
				{
					_sender->message(ERR_NOSUCHNICK(_sender->_servername, _cl_params[i]));
					return (false);
				}
				else if (_sender->_nick == _cl_params[i])
				{
					_sender->message(
					    std::string("You cannot send a message yourself!\n").c_str());
					return (false);
				}
			}
		}
		return (true);
	}

	std::vector<Message> parser(Message *message)
	{
		std::map<size_t, std::string> p = message->getParams();
		if (p.size() < 2)
		{
			throw std::runtime_error(
			    ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, message->getCmd()));
		}

		std::string              msg = std::string(p[1] + "\n").c_str();
		std::vector<std::string> _ch_params = split(p[0], ",");

		std::vector<Message> messages;
		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			std::string tmp(this->getName() + " " + _ch_params[i] + " :" + msg);
			std::cout << tmp << std::endl;
			messages.push_back(Message(tmp));
		}
		return (messages);
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
						clients[j]->message(std::string(":" + _sender->_nick + " PRIVMSG #" +
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
				    std::string(":" + _sender->_nick + " PRIVMSG " + client->getNick() + " :" + msg + "\n")
				        .c_str());
			}
		}
	}
};

#endif
