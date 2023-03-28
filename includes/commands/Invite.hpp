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
	/* ERR_CHANOPRIVSNEEDED
	                    "<channel> :You're not channel operator"

	            - Any command requiring 'chanop' privileges (such as
	              MODE messages) must return this error if the client
	              making the attempt is not a chanop on the specified
	              channel.
	*/
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
		if (channel->joined(client))
		{
			_sender->message(
			    ERR_USERONCHANNEL(_sender->_servername, _sender->_nick, _sender->_username, p[1]));
			return (false);
		}
		return (true);
	}

	void execute()
	{
		_sender->message("invite response\n");
	}
};
#endif
