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
		if (p.size() < 1 || p.size() > 3)
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
			else if (!channel->isOpe(_sender))
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
		_sender->message("Hello world\n");
	}
};
#endif
