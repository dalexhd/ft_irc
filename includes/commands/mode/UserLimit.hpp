#ifndef USER_LIMIT_H
#define USER_LIMIT_H

// Our includes
#include "../Mode.hpp"

class UserLimit : public ChannelModeType
{
  public:
	UserLimit()
	{
		_mode = CHANNEL_MODE_USER_LIMIT;
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
		_channel->setUserLimit(atoll(_params[2].c_str()));
		_channel->addMode(_mode);
	}

	void remove()
	{
		_channel->setUserLimit(MAX_CLIENTS_PER_CHANNEL);
		_channel->removeMode(_mode);
	}
};
#endif
