#ifndef CHANNEL_H
#define CHANNEL_H

#include "./Client.hpp"

enum ChannelMode
{
	CHANNEL_MODE_PRIVATE = 0,                                 //+p OK
	CHANNEL_MODE_SECRET = 1,                                  //+s OK
	CHANNEL_MODE_INVITE_ONLY = 2,                             //+i
	CHANNEL_MODE_MODERATED = 3,                               //+m OK
	CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY = 4, //+t
	CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE = 5,              //+n
	CHANNEL_MODE_BAN_MASK = 6,                                //+b
	CHANNEL_MODE_KEY = 7,                                     //+k
	CHANNEL_MODE_USER_LIMIT = 8,                              //+l OK
	CHANNEL_MODE_OPERATOR = 9,                                //+o
};

enum ClientMode
{
	CLIENT_MODE_VOICE = 0,
	CLIENT_MODE_OPERATOR = 1
};

class Channel
{
  private:
	std::string            _name;
	std::string            _password;
	Client *               _creator;
	time_t                 _created_at;
	std::vector<Message *> _messages;
	long long int          _user_limit;
	std::string            _topic;
	ChannelMode            _mode;
	// New mode implementation...
	std::vector<ChannelMode> _modes;
	std::vector<std::string> _ban_masks;
	std::vector<Client *>    _invite_list;

  public:
	std::vector<Client *>                         _normal_clients;
	std::vector<Client *>                         _voice_clients;
	std::vector<Client *>                         _ope_clients;
	std::vector<std::pair<Client *, ClientMode> > _clients;

  public:
	Channel(std::string &name, std::string &password)
	    : _name(name), _password(password), _creator(NULL), _user_limit(MAX_CLIENTS_PER_CHANNEL)
	{
		_created_at = time(0);
		if (password.size() > 0)
			addMode(CHANNEL_MODE_KEY);
	};

	// Setters
	void setCreator(Client *creator)
	{
		if (_creator)
			throw std::runtime_error("Creator is already assigned");
		_creator = creator;
		_ope_clients.push_back(creator);
	};

	void setTopic(std::string &topic)
	{
		_topic = topic;
	};

	void setPassword(std::string passwd)
	{
		_password = passwd;
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

	char getIdentifier(ChannelMode _mode)
	{
		char identifier = 0;
		switch (_mode)
		{
		case CHANNEL_MODE_BAN_MASK:
			identifier = 'b';
			break;
		case CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE:
			identifier = 'n';
			break;
		case CHANNEL_MODE_INVITE_ONLY:
			identifier = 'i';
			break;
		case CHANNEL_MODE_KEY:
			identifier = 'k';
			break;
		case CHANNEL_MODE_MODERATED:
			identifier = 'm';
			break;
		case CHANNEL_MODE_OPERATOR:
			identifier = 'o';
			break;
		case CHANNEL_MODE_PRIVATE:
			identifier = 'p';
			break;
		case CHANNEL_MODE_SECRET:
			identifier = 's';
			break;
		case CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY:
			identifier = 't';
			break;
		case CHANNEL_MODE_USER_LIMIT:
			identifier = 'l';
			break;
		}
		return (identifier);
	}

	std::string getStringModes(void)
	{
		std::string stringModes = "";
		for (std::vector<ChannelMode>::const_iterator it = _modes.begin();
		     it != _modes.end(); ++it)
		{
			char identifier = getIdentifier(*it);
			if (identifier)
				stringModes += getIdentifier(*it);
		}
		return (stringModes);
	}

	size_t getUserLimit(void)
	{
		return (_user_limit);
	}
	void setUserLimit(size_t limit)
	{
		_user_limit = limit;
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

	bool isBanned(Client *client)
	{
		for (std::vector<std::string>::iterator it = _ban_masks.begin();
		     it != _ban_masks.end(); ++it)
		{
			if (*it == client->getNick() || *it == client->getUsername() ||
			    *it == client->getHost())
				return (true);
		}
		return (false);
	}

	bool isInvited(Client *client)
	{
		return (std::find(_invite_list.begin(), _invite_list.end(), client) !=
		        _invite_list.end());
	}

	bool isInviteOnly(void)
	{
		return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_INVITE_ONLY) !=
		        _modes.end());
	}

	bool isSecret(void)
	{
		return (std::find(_modes.begin(), _modes.end(), CHANNEL_MODE_SECRET) !=
		        _modes.end());
	}

	void addOpe(Client *client)
	{
		std::vector<Client *>::iterator it =
		    std::find(_normal_clients.begin(), _normal_clients.end(), client);
		if (it != _normal_clients.end())
			_normal_clients.erase(it);
		if (std::find(_ope_clients.begin(), _ope_clients.end(), client) ==
		    _ope_clients.end())
			_ope_clients.push_back(client);
	}

	void removeOpe(Client *client)
	{
		std::vector<Client *>::iterator it =
		    std::find(_ope_clients.begin(), _ope_clients.end(), client);
		if (it != _ope_clients.end())
			_ope_clients.erase(it);
		if (std::find(_normal_clients.begin(), _normal_clients.end(), client) ==
		    _normal_clients.end())
			_normal_clients.push_back(client);
	}

	void kick(Client *client)
	{
		std::vector<Client *>::iterator it =
		    std::find(_normal_clients.begin(), _normal_clients.end(), client);
		if (it != _normal_clients.end())
			_normal_clients.erase(it);
		it = std::find(_ope_clients.begin(), _ope_clients.end(), client);
		if (it != _ope_clients.end())
			_ope_clients.erase(it);
		it = std::find(_voice_clients.begin(), _voice_clients.end(), client);
		if (it != _voice_clients.end())
			_voice_clients.erase(it);
	}

	void invite(Client *client)
	{
		if (std::find(_invite_list.begin(), _invite_list.end(), client) ==
		    _invite_list.end())
			this->_invite_list.push_back(client);
	}

	void removeInvite(Client *client)
	{
		std::vector<Client *>::iterator it =
		    std::find(_invite_list.begin(), _invite_list.end(), client);
		if (it != _invite_list.end())
			_invite_list.erase(it);
	}

	void addBanMask(std::string mask)
	{
		if (std::find(_ban_masks.begin(), _ban_masks.end(), mask) == _ban_masks.end())
			this->_ban_masks.push_back(mask);
	}

	void removeBanMask(std::string mask)
	{
		std::vector<std::string>::iterator it =
		    std::find(_ban_masks.begin(), _ban_masks.end(), mask);
		if (it != _ban_masks.end())
			_ban_masks.erase(it);
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
			else if (_modes[i] == CHANNEL_MODE_SECRET)
				mode += "@";
			else if (_modes[i] == CHANNEL_MODE_INVITE_ONLY)
				mode += "!";
			else if (_modes[i] == CHANNEL_MODE_MODERATED)
				mode += "%";
			else if (_modes[i] == CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY)
				mode += "+";
			else if (_modes[i] == CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE)
				mode += "~";
			else if (_modes[i] == CHANNEL_MODE_BAN_MASK)
				mode += "&";
			else if (_modes[i] == CHANNEL_MODE_KEY)
				mode += "!";
		}
		std::cout << "MODE: " << mode << std::endl;
		return (mode);
	}

	bool hasMode(ChannelMode mode)
	{
		return std::find(this->_modes.begin(), this->_modes.end(), mode) !=
		       this->_modes.end();
	}

	void addMode(ChannelMode mode)
	{
		if (!this->hasMode(mode))
			this->_modes.push_back(mode);
	}

	void removeMode(ChannelMode mode)
	{
		std::vector<ChannelMode>::iterator it = std::find(_modes.begin(), _modes.end(), mode);
		if (it != _modes.end())
			_modes.erase(it);
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
				{
					_normal_clients.erase(
					    std::find(_normal_clients.begin(), _normal_clients.end(), client));
				}
			}
		}
	}

	void broadcastMessage(std::string message)
	{
		std::vector<Client *> clients = this->getClients();
		for (size_t i = 0; i < clients.size(); i++)
		{
			clients[i]->message(message);
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
