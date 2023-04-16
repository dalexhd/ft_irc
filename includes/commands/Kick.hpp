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
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();

		if (p.size() > 3 || p.size() < 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->getNick(),
			                                    _message->getCmd())); // ERR_NEEDMOREPARAMS (461)
			return (false);
		}
		else
		{
			std::vector<std::string> _user_params = split(p[1], ",");

			if (p[0][0] != '#')
			{
				_sender->message(ERR_BADCHANMASK(_sender->_servername, _sender->getNick())); // ERR_BADCHANMASK (476)
				return (false);
			}

			Channel *channel = _server->getChannel(p[0]);
			if (channel)
			{
				if (!channel->joined(_sender))
				{
					_sender->message(
					    ERR_NOTONCHANNEL(_sender->_servername, _sender->getNick(), p[0]));
					return (false);
				}
				if (!channel->isOpe(_sender))
				{
					_sender->message(
					    ERR_CHANOPRIVSNEEDED(_sender->_servername, _sender->getNick(), p[0]));
					return (false);
				}
				for (size_t i = 0; i < _user_params.size(); i++)
				{
					Client *user = _server->getClient(_user_params[i]);
					if (!user)
					{
						_sender->message(
						    ERR_NOSUCHNICK(_sender->_servername, _sender->getNick()));
						return (false);
					}
					if (!channel->joined(user))
					{
						_sender->message(ERR_USERNOTINCHANNEL(
						    _sender->_servername, _sender->getNick(), channel->getName())); // ERR_USERNOTINCHANNEL (441)
						return (false);
					}
				}
			}
			else
			{
				_sender->message(ERR_NOSUCHCHANNEL(_sender->_servername, _sender->getNick(), p[0]));
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
			Client *user = _server->getClient(_user_params[i]);
			channel->broadcastMessage(
			    RPL_CUSTOM_KICK(_sender->getUserId(), channel->getName(), user->getNick(), p[2]));
			channel->kick(user);
		}
	};
};
#endif
