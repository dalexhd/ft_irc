#ifndef MODE_H
#define MODE_H

// Our includes
#include "../Command.hpp"

class Mode : public Command
{
  private:
	enum ModeType
	{
		USER_MODE = 0,
		CHANNEL_MODE = 1
	};

	enum ModeSign
	{
		PLUS = 0,
		MINUS = 1
	};

  public:
	Mode()
	{
		_name = "mode";
		_description = "expulsar del servidor";
		_usage = "mode";
		_example[0] = "mode canal> {[+|-]|o|p|s|i|t|n|b|v} [<límite>] "
		              "[<usuario>] [<máscara de ban>]";
		_example[1] = "mode <nick> {[+|-]|i|w|s|o}";
		_is_ope = true;
	}

	// TODO: Signs validation!
	bool validate(void)
	{
		std::map<size_t, std::string> target = _message->getParams();

		if (target.size() < 1)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick, _name)); // ERR_NEEDMOREPARAMS (461)
			return (false);
		}
		else if (target[0].at(0) == '#')
		{
			if (!_server->getChannel(target[0]))
			{
				_sender->message(
				    std::string("Channel" + target[0] + "doesn't exist\n").c_str());
				return (false);
			}
		}
		else
		{
			if (_server->getClient(target[0]) == NULL)
			{
				std::cout << _sender->_nick << target[0] << std::endl;
				_sender->message(ERR_NOSUCHNICK(_sender->_servername, target[0]));
				return (false);
			}
		}
		return (true);
	}

	ModeType getModelType(void)
	{
		std::map<size_t, std::string> target = _message->getParams();

		return target[0].at(0) == '#' ? CHANNEL_MODE : USER_MODE;
	}

	std::vector<std::pair<ModeSign, ChannelMode> > parseChannelModel()
	{
		std::string payload = _message->getParams()[1];
		ModeSign    sign = payload[0] == '+' ? PLUS : MINUS;
		std::vector<std::pair<ModeSign, ChannelMode> > modes;

		for (size_t i = 1; i < payload.size(); i++)
		{
			switch (payload[i])
			{
			case 'p':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_PRIVATE));
				break;
			case 's':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_SECRET));
				break;
			case 'i':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_INVITE_ONLY));
				break;
			case 'm':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_MODERATED));
				break;
			case 't':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_TOPIC_SETTABLE_BY_CHANNEL_OPERATOR_ONLY));
				break;
			case 'n':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_CANT_SENT_MESSAGES_OUTSIDE));
				break;
			case 'b':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_BAN_MASK));
				break;
			case 'k':
				modes.push_back(std::pair<ModeSign, ChannelMode>(sign, CHANNEL_MODE_KEY));
				break;
			}
		}
		return (modes);
	}

	void execute()
	{
		if (getModelType() == CHANNEL_MODE)
		{
			Channel *channel = _server->getChannel(_message->getParams()[0]);
			std::vector<std::pair<ModeSign, ChannelMode> > modes = parseChannelModel();
			std::vector<std::pair<ModeSign, ChannelMode> >::iterator it;

			for (size_t i = 0; i < modes.size(); i++)
			{
				if (modes[i].first == PLUS)
					channel->addMode(modes[i].second);
				else if (modes[i].first == MINUS)
					channel->removeMode(modes[i].second);
			}
		}
	}
};
#endif
