#ifndef NOTICE_H
#define NOTICE_H

// Our includes
#include "../Command.hpp"

class Notice : public Command
{
  public:
	Notice()
	{
		_name = "notice";
		_description = "inicia una conexión con el host especificado. Si no se "
		               "especifica un puerto la conexión será en el puerto "
		               "default de Telnet(21). El host puede ser un nombre o "
		               "ip ";
		_usage = "notice";
		_example[0] = "notice <nick> :<texto>";
		_example[1] = "notice guest500 :oye, eres un imbecil";
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 3)
		{
			_sender->message("Wrong command format. Ex: notice <nick> "
			                 ":<texto>\n");
			return (false);
		}
		return (true);
	}

	void execute()
	{
	}
};
#endif
