#ifndef WHOIS_H
#define WHOIS_H

// Our includes
#include "../Command.hpp"

class Whois : public Command
{
  public:
	Whois()
	{
		_name = "whois";
		_description = "info sobre el usuario usando la mascara";
		_usage = "whois";
		_example[0] = "whois <máscara de nick>[,<máscara de nick>[,...]]";
		_example[1] = "whois guest500";
	}

	void execute()
	{
	}
};
#endif
