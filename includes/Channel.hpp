
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
	enum ChannelMode
	{
		CHANNEL_MODE_PUBLIC = 0,
		CHANNEL_MODE_PRIVATE = 1,
		CHANNEL_MODE_SECRET = 2,
	} _mode;

  public:
	std::vector<Client *> _normal_clients;
	std::vector<Client *> _voice_clients;
	std::vector<Client *> _ope_clients;
	size_t const          _maxClients;

  public:
	// TODO: Should the channel be public or private?
	Channel(std::string &name, std::string &password)
	    : _name(name), _password(password), _creator(NULL), _mode(CHANNEL_MODE_PUBLIC), _maxClients(2)
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

	Client *getCreator(void)
	{
		return (_creator);
	}

	time_t getCreatedAt(void)
	{
		return (_created_at);
	}

	ChannelMode getMode(void)
	{
		return (_mode);
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

	bool isOwner(Client *client)
	{
		return (_creator == client);
	}

	bool isOpe(Client *client)
	{
		return (std::find(_ope_clients.begin(), _ope_clients.end(), client) !=
		        _ope_clients.end());
	}

	bool isNormal(Client *client)
	{
		return (std::find(_normal_clients.begin(), _normal_clients.end(), client) !=
		        _normal_clients.end());
	}

	bool isVoice(Client *client)
	{
		return (std::find(_voice_clients.begin(), _voice_clients.end(), client) !=
		        _voice_clients.end());
	}

	// --------------
	// Utils
	// --------------
	std::string getModeString(void)
	{
		std::string mode;

		if (_mode == CHANNEL_MODE_PRIVATE)
			mode += "*";
		else if (_mode == CHANNEL_MODE_SECRET)
			mode += "@";
		else if (_mode == CHANNEL_MODE_PUBLIC)
			mode += "=";
		return (mode);
	}

	std::string getClientRoleString(Client *client) // TODO: Can a user have multiple roles?
	{
		if (this->isOpe(client))
			return ("@");
		if (this->isVoice(client))
			return ("+");
		return ("");
	}
};

#endif
