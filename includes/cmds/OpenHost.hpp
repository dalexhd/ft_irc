#ifndef OPENHOST_H
#define OPENHOST_H

// Our includes
#include "../Command.hpp"

class Openhost : public Command
{
  public:
	Openhost()
	{
		_name = "open host";
		_description = "inicia una conexión con el host especificado.
		 Si no se especifica un puerto la conexión será en el puerto
		  default de Telnet (21). El host puede ser un nombre o ip";
		_usage = "open host [puerto]";
		_example[0] = "open host [puerto]";
	}

	void execute()
	{

	}
};
#endif
