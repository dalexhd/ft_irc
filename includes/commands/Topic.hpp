#ifndef TOPIC_H
#define TOPIC_H

// Our includes
#include "../Command.hpp"

class Topic : public Command
{
  public:
	Topic()
	{
		_name = "topic";
		_description = "Allows the client to query or set the channel topic on "
		               "channel";
		_usage = "topic";
		_example[0] = "TOPIC #channel1 :another topic";
		_example[1] = "TOPIC #channel1";
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd())); // ERR_NEEDMOREPARAMS (461)
			return (false);
		}

		Channel *channel = _server->getChannel(p[0]);
		if (channel)
		{
			if (!channel->joined(_sender))
			{
				_sender->message(ERR_NOTONCHANNEL(_sender->_servername, _sender->_nick, p[0]));
				return (false);
			}
			else if (!channel->isOpe(_sender) && p.size() > 1)
			{
				_sender->message(
				    ERR_CHANOPRIVSNEEDED(_sender->_servername, _sender->_nick, p[0]));
				return (false);
			}
		}
		else
		{
			_sender->message(ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_nick, p[0]));
			return (false);
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		Channel *                     channel = _server->getChannel(p[0]);

		if (p.size() == 1)
		{
			std::string topic = channel->getTopic();
			if (topic.empty())
				_sender->message(
				    RPL_NOTOPIC(_sender->_servername, _sender->_nick, channel->getName()));
			else
				_sender->message(RPL_TOPIC(_sender->_servername, _sender->_nick,
				                           channel->getName(), channel->getTopic()));
		}
		else
		{
			channel->setTopic(p[1]);
			_sender->message(RPL_TOPIC(_sender->_servername, _sender->_nick,
			                           channel->getName(), channel->getTopic()));
		}
	}
};
#endif
