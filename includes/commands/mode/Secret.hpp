#ifndef SECRET_H
#define SECRET_H

// Our includes
#include "../Mode.hpp"

class Secret : public ChannelModeType
{
  public:
	Secret()
	{
		_mode = CHANNEL_MODE_SECRET;
	}

	void add()
	{
		_channel->addMode(_mode);
	}

	void remove()
	{
		_channel->removeMode(_mode);
	}
};
#endif
