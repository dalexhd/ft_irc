#ifndef KICK_H
#define KICK_H

// Our includes
#include "../Command.hpp"

class Kick : public Command
{
  public:
	Kick()
	{
		_name = "kick";
		_description = "expulsar del servidor";
		_usage = "kick";
		_example[0] = "kick <canal> <usuario> :[<comentario>]";
		_example[1] = "kick #hola user1";
		_example[2] = "kick #hola user1,user2";
		_example[3] = "kick #hola user1,user2 :coment";

		/*
		REPLIES
		ERR_NEEDMOREPARAMS (461) k
		ERR_NOSUCHCHANNEL (403)
		ERR_CHANOPRIVSNEEDED (482)
		ERR_USERNOTINCHANNEL (441) k
		ERR_NOTONCHANNEL (442) k
		ERR_BADCHANMASK (476) k

		*/
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();

		if (p.size() > 3 || p.size() < 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd())); // ERR_NEEDMOREPARAMS (461)
			return (false);
		}
		else
		{
			std::vector<std::string> _user_params = split(p[1], ",");

			if (p[0][0] != '#')
			{
				_sender->message(ERR_BADCHANMASK(_sender->_servername, _sender->_nick)); // ERR_BADCHANMASK (476)
				return (false);
			}

			Channel *channel = _server->getChannel(p[0]);
			if (channel)
			{
				if (!channel->joined(_sender))
				{
					_sender->message(ERR_NOTONCHANNEL(_sender->_servername,
					                                  _sender->_nick,
					                                  p[0])); // ERR_NOTONCHANNEL (442)
					return (false);
				}
				if (!channel->isOpe(_sender))
				{
					_sender->message(ERR_CHANOPRIVSNEEDED(_sender->_servername,
					                                      _sender->_nick,
					                                      p[0])); // ERR_CHANOPRIVSNEEDED (482)
					return (false);
				}
				for (size_t i = 0; i < _user_params.size(); i++)
				{
					Client *user = _server->getClient(_user_params[i]);
					if (!user)
					{
						_sender->message(
						    ERR_NOSUCHNICK(_sender->_servername, _sender->_nick));
						return (false);
					}
					if (!channel->joined(user))
					{
						_sender->message(ERR_USERNOTINCHANNEL(
						    _sender->_servername, _sender->_nick, "#" + channel->getName())); // ERR_USERNOTINCHANNEL (441)
						return (false);
					}
				}
			}
			else
			{
				_sender->message(ERR_NOSUCHCHANNEL(_sender->_servername, _sender->_nick, p[0]));
				return (false);
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();
		std::vector<std::string>      _user_params = split(p[1], ",");

		Channel *channel = _server->getChannel(p[0]);
		for (size_t i = 0; i < _user_params.size(); i++)
		{
			Client *              user = _server->getClient(_user_params[i]);
			std::vector<Client *> related_channels_clients = channel->getClients();
			for (size_t j = 0; j < related_channels_clients.size(); j++)
			{
				related_channels_clients[j]->message(
				    ":" + _sender->_nick + "!" + _sender->_username + "@" +
				    _sender->_host + " KICK #" + channel->getName() + " " + user->_nick + " :" + p[2] + "\n");
			}
			channel->kick(user);
		}
	};
};
#endif
