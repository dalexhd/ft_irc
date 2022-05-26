#ifndef LIST_H
#define LIST_H

// Our includes
#include "../Command.hpp"

class List : public Command
{
  public:
	List()
	{
		_name = "list";
		_description = "listar informacion del canal";
		_usage = "list [<canales>]";
		_example[0] = "list";
		_example[1] = "list #hola";
		_example[2] = "list #hola,#chau";

		// RPL_LISTSTART (321)
		// RPL_LIST (322)
		// RPL_LISTEND (323) k
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() > 1)
		{
			_sender->message("Wrong command format. Ex: list "
			                 "[<canal>{,<canal>}]\n");
			return (false);
		}
		std::vector<std::string> _ch_params = split(p[0], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			if (_ch_params[i][0] != '#')
			{
				ERR_BADCHANMASK(_sender->_servername,_sender->_name); // ERR_BADCHANMASK (476)
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
		if (!validate())
			return;
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() == 1 )
		{
			std::vector<std::string> _ch_params = split(p[0], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					_sender->message(std::string("Channel: " + channel->getName() + " " + (channel->joined(_sender) ? "(Joined)" : "") + "\n")
										.c_str());
				}
				else
					ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_name, _ch_params[i]);
			}

		}
		else
		{
			std::vector<Channel *> channels = _server->getChannels();
			for (size_t i = 0; i < channels.size(); i++)
			{

				_sender->message(std::string(_sender->_name + " Channel :Users  Name\n")
				                     .c_str());
				//"<client> <channel> <client count> :<topic>"
				_sender->message(std::string(_sender->_name + " " + channels[i]->getName() + " " + itoa((int)channels[i]->_normal_clients.size() + (int)channels[i]->_ope_clients.size()) + " : " + "<Topic>" +"\n")
				                     .c_str());
				_sender->message(std::string(_sender->_name + " : End of /LIST\n").c_str());
			}
		}
	}
};
#endif
