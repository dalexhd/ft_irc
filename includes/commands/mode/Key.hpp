#ifndef KEY_H
#define KEY_H

// Our includes
#include "../Mode.hpp"

class Key : public ChannelModeType
{
  public:
	Key()
	{
		_mode = CHANNEL_MODE_KEY;
	}

	bool validate(void)
	{
		if (_params.size() < 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, "mode"));
			return false;
		}
		return true;
	}

	void add()
	{
		_channel->setPassword(_params[2]);
		_channel->addMode(_mode);
	}

	void remove()
	{
		_channel->setPassword("");
		_channel->removeMode(_mode);
	}
};
#endif
