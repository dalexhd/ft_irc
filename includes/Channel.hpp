#ifndef CHANNEL_H
#define CHANNEL_H

#include "./Client.hpp"

enum ChannelMode
{
	CHANNEL_MODE_PRIVATE = 0,
	CHANNEL_MODE_SECRET = 1,
	CHANNEL_MODE_INVITE_ONLY = 2,
	CHANNEL_MODE_MODERATED = 3,
	CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY = 4,
	CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE = 5,
	CHANNEL_MODE_BAN_MASK = 6,
	CHANNEL_MODE_KEY = 7,
	CHANNEL_MODE_USER_LIMIT = 8,
	CHANNEL_MODE_OPERATOR = 9,
};

class Channel
{
  private:
	std::string            _name;
	std::string            _password;
	Client *               _creator;
	time_t                 _created_at;
	std::vector<Message *> _messages;
	long long int          _max_clients;
	std::string            _topic;
	ChannelMode            _mode;
	// New mode implementation...
	std::vector<ChannelMode> _modes;

  public:
	std::vector<Client *> _normal_clients;
	std::vector<Client *> _voice_clients;
	std::vector<Client *> _ope_clients;

  public:
	Channel(std::string &name, std::string &password)
	    : _name(name), _password(password), _creator(NULL), _max_clients(MAX_CLIENTS_PER_CHANNEL)
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

	void setTopic(std::string &topic)
	{
		_topic = topic;
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

	std::vector<ChannelMode> getModes(void)
	{
		return (_modes);
	}
	size_t getMaxClients(void)
	{
		return (_max_clients);
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
		for (size_t i = 0; i < _modes.size(); i++)
		{
			if (_modes[i] == CHANNEL_MODE_PRIVATE)
				mode += "*";
			if (_modes[i] == CHANNEL_MODE_SECRET)
				mode += "@";
			if (_modes[i] == CHANNEL_MODE_INVITE_ONLY)
				mode += "!";
			if (_modes[i] == CHANNEL_MODE_MODERATED)
				mode += "%";
			if (_modes[i] == CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY)
				mode += "+";
			if (_modes[i] == CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE)
				mode += "~";
			if (_modes[i] == CHANNEL_MODE_BAN_MASK)
				mode += "&";
			if (_modes[i] == CHANNEL_MODE_KEY)
				mode += "!";
		}
		return (mode);
	}

	void addMode(ChannelMode mode)
	{
		this->_modes.push_back(mode);
		std::cout << "Added mode" << mode << std::endl;
	}

	void removeMode(ChannelMode mode)
	{
		std::vector<ChannelMode>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
		if (it != _modes.end())
		{
			_modes.erase(it);
			std::cout << "Removed mode" << mode << std::endl;
		}
		else
			std::cout << "Could not Remove mode" << mode << std::endl;
	}

	std::string getClientRoleString(Client *client) // TODO: Can a user have multiple roles?
	{
		if (this->isOpe(client))
			return ("@");
		if (this->isVoice(client))
			return ("+");
		return ("");
	}

	void removeClientFromChannel(Client *client)
	{
		std::vector<Client *> related_channels_clients = getClients();
		for (size_t j = 0; j < related_channels_clients.size(); j++)
		{
			if (related_channels_clients[j] == client)
			{
				if (isOpe(related_channels_clients[j]))
					_ope_clients.erase(
					    std::find(_ope_clients.begin(), _ope_clients.end(), client));
				else
					_normal_clients.erase(
					    std::find(_normal_clients.begin(), _normal_clients.end(), client));
			}
		}
	}

	bool isModerated(void)
	{
		return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_MODERATED) !=
		        _modes.end());
	}

	bool hasKey(void)
	{
		return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_KEY) !=
		        _modes.end());
	}
};

#endif
