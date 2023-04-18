#ifndef MODERATED_H
#define MODERATED_H

// Our includes
#include "../Mode.hpp"

class Moderated : public ChannelModeType
{
  public:
	Moderated()
	{
		_mode = CHANNEL_MODE_MODERATED;
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
