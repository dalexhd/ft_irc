#ifndef TOPIC_H
#define TOPIC_H

// Our includes
#include "../Command.hpp"

class Topic : public Command
{
  public:
	Topic()
	{
		_name = "topic";
		_description = "Allows the client to query or set the channel topic on channel";
		_usage = "topic";
		_example[0] = "TOPIC #channel1 :another topic";
		_example[1] = "TOPIC #channel1";
	}

	void execute()
	{
		_sender->message("Hello world\n");
	}
};
#endif
