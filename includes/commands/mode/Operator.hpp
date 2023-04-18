#ifndef OPERATOR_H
#define OPERATOR_H

// Our includes
#include "../Mode.hpp"

class Operator : public ChannelModeType
{
  public:
	Operator()
	{
		_mode = CHANNEL_MODE_OPERATOR;
	}

	bool validate(void)
	{
		if (_params.size() != 3)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, "mode"));
			return false;
		}

		Client *client = _server->getClient(_params[2]);
		if (client == NULL)
		{
			_sender->message(ERR_NOSUCHNICK(_sender->_servername, _params[2]));
			_sender->message(ERR_USERNOTINCHANNEL(_sender->_servername, _params[2], _params[0]));
			return false;
		}
		return true;
	}

	void add()
	{
		Client *client = _server->getClient(_params[2]);
		_channel->addOpe(client);
		_channel->addMode(_mode);
	}

	void remove()
	{
		Client *client = _server->getClient(_params[2]);
		_channel->removeOpe(client);
		_channel->removeMode(_mode);
	}
};
#endif
