#ifndef INVITE_H
#define INVITE_H

// Our includes
#include "../Command.hpp"

class Invite : public Command
{
  public:
	Invite()
	{
		_name = "invite";
		_description = "invita usuario a canal";
		_usage = "invite";
		_example[0] = "invite <nick> <canal>";
		//		_is_opec = true;
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();

		if (p.size() < 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd()));
			return (false);
		}
		Client *client = _server->getClient(p[0]);
		if (client == NULL)
		{
			_sender->message(ERR_NOSUCHNICK(_sender->_servername, _sender->_nick));
			return (false);
		}
		Channel *channel = _server->getChannel(p[1]);
		if (!channel || !channel->joined(_sender))
		{
			_sender->message(ERR_NOTONCHANNEL(_sender->_servername, _sender->_nick, p[1]));
			return (false);
		}
		else if (channel->joined(client))
		{
			_sender->message(
			    ERR_USERONCHANNEL(_sender->_servername, _sender->_nick, _sender->_username, p[1]));
			return (false);
		}
		else if (!channel->isOpe(_sender) && !channel->hasMode(CHANNEL_MODE_INVITE_ONLY))
		{
			_sender->message(ERR_CHANOPRIVSNEEDED(_sender->_servername, _sender->_nick, p[1]));
			return (false);
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();

		Channel *channel = _server->getChannel(p[1]);
		Client * client = _server->getClient(p[0]);

		client->message(RPL_INVITING(_sender->_servername, _sender->getNick(),
		                             channel->getName(), client->getUsername()));
		client->message(
		    RPL_CUSTOM_INVITE(_sender->getUserId(), channel->getName(), client->getUsername()));
		channel->invite(client);
	}
};
#endif
