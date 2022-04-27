#ifndef AWAY_H
#define AWAY_H

// Our includes
#include "../Command.hpp"

class Away : public Command
{
  public:
	Away()
	{
		_name = "away";
		_description = "respuesta automatica a privmsg";
		_usage = "away";
		_example[0] = "away :[mensaje]";
	}

	void execute()
	{

	}
};
#endif
