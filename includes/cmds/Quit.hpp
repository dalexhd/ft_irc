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
		std::vector<Client *> clients = _server->getRelatedClients(_sender);
		for (size_t i = 0; i < clients.size(); i++)
		{
			clients[i]->message(std::string(":" + _sender->_nick + "!" + _sender->_username + "@" +
			                                _sender->_servername +
			                                " QUIT :" + _message->getParams()[0] + "\n")
			                        .c_str());
		}

		std::map<std::string, Channel *> related_channels = _server->getRelatedChannels(_sender);
		std::map<std::string, Channel *>::iterator it = related_channels.begin();
		for (; it != related_channels.end(); it++)
		{
			it->second->removeClientFromChannel(_sender);
		}
		this->_server->deleteClient(_sender->_fd);
	}
};
#endif
