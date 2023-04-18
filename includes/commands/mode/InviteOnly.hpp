#ifndef INVITE_ONLY_H
#define INVITE_ONLY_H

// Our includes
#include "../Mode.hpp"

class InviteOnly : public ChannelModeType
{
  public:
	InviteOnly()
	{
		_mode = CHANNEL_MODE_INVITE_ONLY;
	}

	bool validate(void)
	{
		if (_params.size() < 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, "mode"));
			return false;
		}
		return true;
	}

	void add()
	{
		_channel->addMode(CHANNEL_MODE_INVITE_ONLY);
	}

	void remove()
	{
		_channel->removeMode(CHANNEL_MODE_INVITE_ONLY);
	}
};
#endif
