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

	void add()
	{
	}

	void remove()
	{
	}
};
#endif
