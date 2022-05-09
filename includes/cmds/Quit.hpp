#ifndef QUIT_H
#define QUIT_H

// Our includes
#include "../Command.hpp"

class Quit : public Command
{
  public:
	Quit()
	{
		_name = "quit";
		_description = "salir del servidor con mensaje de salida";
		_usage = "quit";
		_example[0] = "quit :[<Mensaje de salida>]";
		_example[1] = "quit :Hasta la vista, baby";
	}

	void execute()
	{

	}
};
#endif
