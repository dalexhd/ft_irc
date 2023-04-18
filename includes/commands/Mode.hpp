#ifndef MODE_H
#define MODE_H

// Our includes
#include "../Command.hpp"

enum ModeSign
{
	PLUS = 0,
	MINUS = 1
};

class ChannelModeType
{
  public:
	ChannelMode                   _mode;
	char                          _identifier;
	ModeSign                      _sign;
	Channel *                     _channel;
	std::map<size_t, std::string> _params;
	Client *                      _sender;
	Server *                      _server;

  public:
	ChannelModeType(){};

	void execute()
	{
		if (_sign == PLUS)
			add();
		else
			remove();

		_channel->broadcastMessage(
		    RPL_CUSTOM_MODE(_sender->getUserId(), _channel->getName(),
		                    (_sign == PLUS ? "+" : "-") + _channel->getIdentifier(_mode) + " " + _params[2]));
	}

	void setMode(ChannelMode mode)
	{
		_mode = mode;
	}

	void setSign(ModeSign sign)
	{
		_sign = sign;
	}

	void setChannel(Channel *channel)
	{
		_channel = channel;
	}

	void setParams(std::map<size_t, std::string> params)
	{
		_params = params;
	}

	void setSender(Client *sender)
	{
		_sender = sender;
	}

	void setServer(Server *server)
	{
		_server = server;
	}

	virtual bool validate(void)
	{
		return (true);
	}
	virtual void add() = 0;
	virtual void remove() = 0;

	virtual ~ChannelModeType()
	{
	}
};

#include "./mode/BanMask.hpp"
#include "./mode/CantSentMessagesOutside.hpp"
#include "./mode/InviteOnly.hpp"
#include "./mode/Key.hpp"
#include "./mode/Moderated.hpp"
#include "./mode/Operator.hpp"
#include "./mode/Secret.hpp"
#include "./mode/TopicSettableByChannelOperatorOnly.hpp"
#include "./mode/UserLimit.hpp"

class Mode : public Command
{
  private:
	enum Type
	{
		USER_MODE = 0,
		CHANNEL_MODE = 1
	};

  private:
	Type getModelType(void)
	{
		std::map<size_t, std::string> target = _message->getParams();

		return target[0].at(0) == '#' ? CHANNEL_MODE : USER_MODE;
	}

	std::vector<ChannelModeType *> parseChannelMode()
	{
		std::map<size_t, std::string> params = _message->getParams();
		Channel *                     channel = _server->getChannel(params[0]);
		std::string                   modeParams = params[1];
		ModeSign sign = modeParams[0] == '+' ? PLUS : MINUS;
		std::vector<ChannelModeType *> modes;

		for (size_t i = 1; i < modeParams.size(); i++)
		{
			ChannelModeType *mode = NULL;
			switch (modeParams[i])
			{
			case 'o':
				mode = new Operator();
				break;
			case 'p':
				mode = new CantSentMessagesOutside();
				break;
			case 's':
				mode = new Secret();
				break;
			case 'i':
				mode = new InviteOnly();
				break;
			case 'm':
				mode = new Moderated();
				break;
			case 't':
				mode = new TopicSettableByChannelOperatorOnly();
				break;
			case 'n':
				mode = new CantSentMessagesOutside();
				break;
			case 'b':
				mode = new BanMask();
				break;
			case 'k':
				mode = new Key();
				break;
			case 'l':
				mode = new UserLimit();
				break;
			default:
				break;
			}
			if (mode != NULL)
			{
				mode->setSign(sign);
				mode->setParams(params);
				mode->setChannel(&(*channel));
				mode->setSender(&(*_sender));
				mode->setServer(&(*_server));
				modes.push_back(mode);
			}
		}
		return (modes);
	}

  public:
	Mode()
	{
		_name = "mode";
		_description = "add or remove a mode to a channel or user";
		_usage = "mode <canal> {[+|-]|o|p|s|i|t|n|b|v} [<límite>] "
		         "[<usuario>] [<máscara de ban>]";
		_example[0] = "mode #canal +o nick";
		_example[1] = "mode #canal +l 10";
		_example[1] = "mode #canal +ts";
		_example[2] = "mode roberto +it";
		//_is_ope = true;
	}

	bool validate()
	{
		std::map<size_t, std::string> params = _message->getParams();

		if (params.size() < 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, _name));
			return (false);
		}
		Channel *_channel = _server->getChannel(params[0]);
		if (_channel == NULL)
		{
			_sender->message(ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_nick, params[0]));
			return (false);
		}
		else if (params.size() > 2 && _channel->isOpe(_sender) == false)
		{
			_sender->message(ERR_CHANOPRIVSNEEDED(_sender->_servername, _sender->_nick,
			                                      _channel->getName()));
			return (false);
		}

		std::vector<ChannelModeType *> modes = parseChannelMode();
		for (size_t i = 0; i < modes.size(); i++)
		{
			if (modes[i]->validate() == false)
			{
				delete modes[i];
				return (false);
			}
			delete modes[i];
		}
		modes.clear();
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> params = _message->getParams();

		if (params.size() == 1)
		{
			Channel *channel = _server->getChannel(params[0]);
			_sender->message(RPL_CHANNELMODEIS(_sender->_servername, _sender->_nick,
			                                   channel->getName(), channel->getStringModes()));
			return;
		}
		else
		{
			Type type = getModelType();
			if (type == CHANNEL_MODE)
			{
				std::vector<ChannelModeType *> modes = parseChannelMode();
				std::vector<ChannelModeType *>::iterator it;

				for (size_t i = 0; i < modes.size(); i++)
				{
					modes[i]->execute();
					delete modes[i];
				}

				modes.clear();
			}
			else if (type == USER_MODE)
			{
			}
		}
	}
};
#endif
