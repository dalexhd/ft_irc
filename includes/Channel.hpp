
#ifndef CHANNEL_H
#define CHANNEL_H

#include "./Client.hpp"
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
	std::string            _topic;

  public:
	std::vector<Client *> _normal_clients;
	std::vector<Client *> _ope_clients;
	size_t const          _maxClients;

  public:
	Channel(std::string &name, std::string &password)
	    : _name(name), _password(password), _creator(NULL), _maxClients(2)
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

	std::string &getTopic(void)
	{
		return (_topic);
	}

	std::vector<Client *> getClients(void) const
	{
		std::vector<Client *> clients;
		clients.insert(clients.end(), _normal_clients.begin(), _normal_clients.end());
		clients.insert(clients.end(), _ope_clients.begin(), _ope_clients.end());
		return (clients);
	}

	// --------------
	// Client stuff
	// --------------
	bool joined(Client *client)
	{
		std::vector<Client *> clients = this->getClients();

		return (std::find(clients.begin(), clients.end(), client) != clients.end());
	}
};

#endif
