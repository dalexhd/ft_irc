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
	}

	void remove()
	{
	}
};
#endif
