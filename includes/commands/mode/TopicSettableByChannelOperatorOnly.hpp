#ifndef TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY_H
#define TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY_H

// Our includes
#include "../Mode.hpp"

class TopicSettableByChannelOperatorOnly : public ChannelModeType
{
  public:
	TopicSettableByChannelOperatorOnly()
	{
		_mode = CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY;
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
