#ifndef CAN_SENT_MESSAGES_OUTSIDE_H
#define CAN_SENT_MESSAGES_OUTSIDE_H

// Our includes
#include "../Mode.hpp"

class CantSentMessagesOutside : public ChannelModeType
{
  public:
	CantSentMessagesOutside()
	{
		_mode = CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE;
	}

	void add()
	{
	}

	void remove()
	{
	}
};
#endif
