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

		// ERR_NEEDMOREPARAMS (461) k
		// ERR_NOSUCHCHANNEL (403)
		// ERR_TOOMANYCHANNELS (405)
		// ERR_BADCHANNELKEY (475)
		// ERR_BANNEDFROMCHAN (474)
		// ERR_CHANNELISFULL (471) k
		// ERR_INVITEONLYCHAN (473)
		// RPL_TOPIC (332)
		// RPL_TOPICWHOTIME (333)
		// RPL_NAMREPLY (353)
		// RPL_ENDOFNAMES (366)

	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1 || p.size() > 2)
		{
			//ERR_NEEDMOREPARAMS (461)
			_sender->message(_sender->_name + " " + _message->getCmd() + " :Wrong num of parameters\n");
			return (false);
		}
		else
		{
			std::vector<std::string> _ch_params = split(p[0], ",");
			std::vector<std::string> _pw_params = split(p[1], ",");

			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				if (_ch_params[i][0] != '#')
				{
					//ERR_BADCHANMASK (476)
					_sender->message(_ch_params[i] + " :Bad Channel Mask\n");
					return (false);
				}
			}
			if (_pw_params.size() > 0 && _pw_params.size() != _ch_params.size())
			{
				_sender->message("Wrong command format. Ex: join "
				                 "#canal1,#canal2 "
				                 "clave1,clave2\n");
				return (false);
			}
			for (size_t i = 0; i < _ch_params.size(); i++)
			{
				Channel *channel = _server->getChannel(_ch_params[i]);
				if (channel)
				{
					if (channel->joined(_sender))
					{
						_sender->message("You're already member of that "
						                 "channel!\n");
						return (false);
					}
				}
			}
		}
		return (true);
	}

	void execute()
	{
		if (!validate())
			return;
		std::map<size_t, std::string> p = _message->getParams();

		std::vector<std::string> _ch_params = split(p[0], ",");
		std::vector<std::string> _pw_params = split(p[1], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			Channel *channel = _server->getChannel(_ch_params[i]);

			if (channel)
			{

				if(((int)channel->_normal_clients.size() + (int)channel->_ope_clients.size()) > (int)channel->_maxClients)
						_sender->message(_sender->_name + " " + _ch_params[i] + " :Cannot join channel , channel is full(+l)\n"); //ERR_CHANNELISFULL (471)
				else if (_pw_params.size() > 0)
				{
					if (channel->getPassword() == _pw_params[i])
					{
						channel->_normal_clients.push_back(_sender);
						//std::cout << itoa((int)channel->_normal_clients.size()) << std::endl;
						_sender->message(std::string("Client " + _sender->_name + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
					}
					else
						_sender->message("Wrong password\n"); // ERR_BADCHANNELKEY (475)
				}
				else
				{
					if (channel->getPassword() != "")
						_sender->message("Wrong password\n"); // ERR_BADCHANNELKEY (475)
					else
					{
						channel->_normal_clients.push_back(_sender);
						for(size_t j = 0; j < channel->_normal_clients.size(); j++)
						{
							if(_sender->_name != channel->_normal_clients[j]->_name)
								channel->_normal_clients[j]->message(std::string(_sender->_name + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
						}
						for(size_t j = 0; j < channel->_ope_clients.size(); j++)
						{
							if(_sender->_name != channel->_ope_clients[j]->_name)
								channel->_ope_clients[j]->message(std::string(_sender->_name + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
						}
					}
				}
			}
			else
			{

				Channel *channel;
				/*std::vector<Channel *> channels = _server->getChannels();
				int j = 0;

				if(channels.size() > 1) // err: why channels size is 1
				{
					std::cout << "sender channels" << itoa(channels.size() - 1) << std::endl;
					for (size_t i = 0; i < channels.size() - 1; i++)
					{
						//std::cout << channels[i]->getName() << std::endl;
						if(channels[i]->joined(_sender))
							j++;


					}
				}
				if(j > _sender->_maxChannels)
				{
						_sender->message(_sender->_name + " " + _ch_params[i] + " :You have joined too many channels\n"); //ERR_TOOMANYCHANNELS (405)
						return;
				}
				else */if (_pw_params.size() > 0)
				{
					channel = _server->createChannel(_ch_params[i], _pw_params[i]);
					_sender->message(std::string("Channel " + _ch_params[i] + " with password \"" + _pw_params[i] + "\" created\n")
					                     .c_str());
				}
				else
				{
					std::string pwd = std::string("");
					channel = _server->createChannel(_ch_params[i], pwd);
					_sender->message(std::string("Channel " + _ch_params[i] + " with empty password created\n")
					                     .c_str());
				}

				channel->setCreator(_sender);
				// :marc459!m@127.0.0.1 JOIN :#channel
				// RPL_TOPIC (332) marc459 #channel :No topic is set
				// RPL_TOPICWHOTIME (333)
				// RPL_NAMREPLY (353) 353 marc459 = #channel :@marc459
				// RPL_ENDOFNAMES (366) 366 marc459 #channel :End of /NAMES list.
				_sender->message(std::string( _sender->_name + "!<user_name>@<server_host> JOIN :" + _ch_params[i] + "\n")
				                     .c_str());
				_sender->message(std::string( _sender->_name + " #" + _ch_params[i] + ":No topic set" + "\n")
				                     .c_str());
				std::map<std::string, Command *>::iterator it;
				channel->_ope_clients.push_back(_sender);
				if ((it = _server->_commands.find("names")) != _server->_commands.end())
				{
					Command *cmd = it->second;
					std::string names = "names";
					Message *message = new Message(names);
					cmd->setSender(_sender, i - 1);
					cmd->setServer(_server);
					cmd->setMessage(message);
					cmd->execute();
				}

			}
		}
	}
};
#endif
