#ifndef PRIVATE_H
#define PRIVATE_H

// Our includes
#include "../Mode.hpp"

class Private : public ChannelModeType
{
  public:
	Private()
	{
		_mode = CHANNEL_MODE_PRIVATE;
	}

	void add()
	{
	}

	void remove()
	{
	}
};
#endif
