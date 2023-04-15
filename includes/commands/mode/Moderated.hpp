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
	}

	void remove()
	{
	}
};
#endif
