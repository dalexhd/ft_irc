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
		_channel->addMode(_mode);
	}

	void remove()
	{
		_channel->removeMode(_mode);
	}
};
#endif
