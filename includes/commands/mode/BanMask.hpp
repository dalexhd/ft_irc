#ifndef BAN_MASK_H
#define BAN_MASK_H

// Our includes
#include "../Mode.hpp"

class BanMask : public ChannelModeType
{
  public:
	BanMask()
	{
		_mode = CHANNEL_MODE_BAN_MASK;
	}

	void add()
	{
		_channel->addBanMask(_params[2]);
	}

	void remove()
	{
		_channel->removeBanMask(_params[2]);
	}
};
#endif
