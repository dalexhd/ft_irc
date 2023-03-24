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
		_is_ope = true;

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
		bool                          getUserChannel;
		bool                          isSenderOnChannel;

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
				_sender->message("Wrong command format. Ex: list "
				                 "#canal1 "
				                 "user\n");
				return (false);
			}

			Channel *channel = _server->getChannel(p[0]);
			if (channel)
			{
				for (size_t i = 0; i < _user_params.size(); i++)
				{
					getUserChannel = false;
					isSenderOnChannel = false;
					for (size_t j = 0; j < channel->_normal_clients.size(); j++)
					{
						if (channel->_normal_clients[j]->_nick == _user_params[i])
							getUserChannel = true;
						if (channel->_normal_clients[j]->_nick == _sender->_nick)
							isSenderOnChannel = true;
					}
					for (size_t j = 0; j < channel->_ope_clients.size(); j++)
					{
						if (channel->_ope_clients[j]->_nick == _user_params[i])
							getUserChannel = true;
						if (channel->_ope_clients[j]->_nick == _sender->_nick)
							isSenderOnChannel = true;
					}

					if (getUserChannel == false)
					{
						_sender->message(
						    ERR_USERNOTINCHANNEL(_sender->_servername, _sender->_nick, p[0]));
						return (false);
					}
					if (isSenderOnChannel == false)
					{
						_sender->message(
						    ERR_NOTONCHANNEL(_sender->_servername, _sender->_nick, p[0]));
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
		if (channel)
		{
			for (size_t i = 0; i < _user_params.size(); i++)
			{
				for (size_t j = 0; j < channel->_normal_clients.size(); j++)
				{
					if (channel->_normal_clients[j]->_nick == _user_params[i])
					{
						channel->_normal_clients.erase(channel->_normal_clients.begin() + j);
						// channel->_normal_clients[j]->message(std::string(_sender->_name + " kicked you from  " + channel->getName() + "\n").c_str());
					}
				}
				for (size_t j = 0; j < channel->_ope_clients.size(); j++)
				{
					if (channel->_ope_clients[j]->_nick == _user_params[i])
						channel->_ope_clients.erase(channel->_ope_clients.begin() + j);
				}
			}
		}
	}
};
#endif
