#ifndef JOIN_H
#define JOIN_H

// Our includes
#include "../Command.hpp"

class Join : public Command
{
  public:
	Join()
	{
		_name = "join";
		_description = "une al usuario al canal si existe o genera un nuevo "
		               "canal";
		_usage = "join <canal>{,<canal>} [<clave>{,<clave>}]";
		_example[0] = "join #canal1 clave1";
		_example[1] = "join #canal1,#canal2 clave1,clave2";

		// ERR_NOSUCHCHANNEL (403)
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1 || p.size() > 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd()));
			return (false);
		}
		else
		{
			if (p[0][0] != '#')
			{
				_sender->message(ERR_BADCHANMASK(_sender->_servername, _sender->_nick));
				return (false);
			}
			if (_server->getRelatedChannels(_sender).size() >= MAX_CHANNELS)
			{
				_sender->message(
				    ERR_TOOMANYCHANNELS(_sender->_servername, _sender->getNick(), p[0]));
				return (false);
			}

			Channel *channel = _server->getChannel(p[0]);
			if (channel)
			{
				if (channel->joined(_sender))
				{
					_sender->message(
					    ERR_USERONCHANNEL(_sender->_servername, _sender->getNick(),
					                      _sender->getUsername(), channel->getName())); // ERR_USERONCHANNEL 443
					return (false);
				}
				else if ((channel->_normal_clients.size() +
				          channel->_ope_clients.size()) > channel->getUserLimit())
				{
					_sender->message(ERR_CHANNELISFULL(
					    _sender->_servername, _sender->getNick(), channel->getName())); // ERR_CHANNELISFULL 471
					return (false);
				}
				else if ((channel->getPassword() != "" && (p.size() == 0 || channel->getPassword() != p[1])))
				{
					_sender->message(ERR_BADCHANNELKEY(
					    _sender->_servername, _sender->getNick(), channel->getName())); // ERR_BADCHANNELKEY 475
					return (false);
				}
				else if (channel->isBanned(_sender))
				{
					_sender->message(ERR_BANNEDFROMCHAN(
					    _sender->_servername, _sender->getNick(), channel->getName())); // ERR_BANNEDFROMCHAN 474
					return (false);
				}
				else if (channel->isInviteOnly() && !channel->isInvited(_sender))
				{
					_sender->message(ERR_INVITEONLYCHAN(
					    _sender->_servername, _sender->getNick(), channel->getName())); // ERR_INVITEONLYCHAN 473
					return (false);
				}
			}
		}
		return (true);
	}

	std::vector<Message> parser(Message *message)
	{
		std::map<size_t, std::string> p = message->getParams();
		if (p.size() < 1)
		{
			throw std::runtime_error(ERR_NEEDMOREPARAMS(
			    _sender->_servername, _sender->getNick(), message->getCmd()));
		}

		std::vector<std::string> _ch_params = split(p[0], ",");
		std::vector<std::string> _pw_params = split(p[1], ",");
		std::vector<Message>     messages;
		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			std::string _ch_param = _ch_params[i];
			if (_pw_params.size() > 0 && i < _pw_params.size())
			{
				std::string tmp(std::string(this->getName() + " " + _ch_param + " " + _pw_params[i]));
				messages.push_back(Message(tmp));
			}
			else
			{
				std::string tmp(std::string(this->getName() + " " + _ch_param));
				messages.push_back(Message(tmp));
			}
		}

		return (messages);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();

		Channel *channel = _server->getChannel(p[0]);

		if (channel)
		{
			channel->_normal_clients.push_back(_sender);
			channel->broadcastMessage(RPL_CUSTOM_JOIN(_sender->_nick, channel->getName()));
			std::string topic = channel->getTopic();
			_sender->message(RPL_TOPIC(_sender->_servername, _sender->_nick,
			                           channel->getName(),
			                           (topic.size() > 0 ? topic : "No topic is set"))); // RPL_TOPIC 332

			std::vector<Client *> clients = channel->getClients();
			std::string           users_str = "";
			for (size_t i = 0; i < clients.size(); i++)
			{
				users_str +=
				    channel->getClientRoleString(clients[i]) + clients[i]->getNick() + (i == clients.size() - 1 ? "" : " ");
			}
			_sender->message(RPL_NAMREPLY(_sender->_servername, _sender->_nick,
			                              channel->getModeString(), channel->getName(), users_str));
			_sender->message(
			    RPL_ENDOFNAMES(_sender->_servername, _sender->_nick, channel->getName()));
		}
		else
		{
			Channel *   channel;
			std::string password = p.size() > 0 ? p[1] : std::string("");
			channel = _server->createChannel(p[0], password);
			channel->setCreator(_sender);
			_sender->message(RPL_CUSTOM_JOIN(_sender->getUserId(), channel->getName()));
			_sender->message(
			    RPL_TOPIC(_sender->_servername, _sender->_nick, channel->getName(), "No topic is set"));
			_sender->message(RPL_NAMREPLY(_sender->_servername, _sender->getNick(), "%", p[0],
			                              "@" + _sender->getNick()));
			_sender->message(
			    RPL_ENDOFNAMES(_sender->_servername, _sender->_nick, channel->getName()));
		}
	}
};
#endif
