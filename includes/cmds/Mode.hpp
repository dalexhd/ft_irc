#ifndef MODE_H
#define MODE_H

// Our includes
#include "../Command.hpp"

class Mode : public Command
{
  public:
	Mode()
	{
		_name = "mode";
		_description = "expulsar del servidor";
		_usage = "mode";
		_example[0] = "mode canal> {[+|-]|o|p|s|i|t|n|b|v} [<límite>] "
		              "[<usuario>] [<máscara de ban>]";
		_example[1] = "mode <nick> {[+|-]|i|w|s|o}";
		_is_ope = true;
	}

	void execute()
	{
	}
};
#endif
