
#ifndef CHANNEL_H
#define CHANNEL_H

#include "./client.hpp"
#include <ctime>
#include <vector>

class Channel
{
  private:
	std::string _name; // Debe empezar por #
	std::string _password;

	Client *               _creator;
	time_t                 _created_at;
	std::vector<Message *> _messages;

  public:
	std::vector<Client *> _normal_clients;
	std::vector<Client *> _ope_clients;

  public:
	Channel(std::string &name, std::string &password)
	    : _name(name), _password(password), _creator(NULL)
	{
		_created_at = time(0);
	};

	// Setters
	void setCreator(Client *creator)
	{
		if (_creator)
			throw std::runtime_error("Creator is already assigned");
		_creator = creator;
	};

	// Getter
	std::string &getPassword(void)
	{
		return (_password);
	}

	std::string &getName(void)
	{
		return (_name);
	}

	// --------------
	// Client stuff
	// --------------
	bool joined(Client *client)
	{
		for (size_t i = 0; i < this->_normal_clients.size(); i++)
			if (this->_normal_clients[i]->_name == client->_name)
				return (true);
		for (size_t i = 0; i < this->_ope_clients.size(); i++)
			if (this->_ope_clients[i]->_name == client->_name)
				return (true);
		return (false);
	}
};

#endif
