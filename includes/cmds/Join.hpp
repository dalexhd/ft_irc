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
		// RPL_TOPIC (332) k
		// RPL_TOPICWHOTIME (333)
		// RPL_NAMREPLY (353) k
		// RPL_ENDOFNAMES (366)k k
	}

	bool validate(void)
	{
		std::map<size_t, std::string> p = _message->getParams();
		if (p.size() < 1 || p.size() > 2)
		{
			_sender->message(ERR_NEEDMOREPARAMS(_sender->_servername, _sender->_nick,
			                                    _message->getCmd())); // ERR_NEEDMOREPARAMS (461)
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
					_sender->message(
					    ERR_BADCHANMASK(_sender->_servername, _sender->_nick)); // ERR_BADCHANMASK (476)
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
						_sender->message(
						    ERR_USERONCHANNEL(_sender->_servername, _sender->_nick)); // ERR_USERONCHANNEL 443
						return (false);
					}
				}
			}
		}
		return (true);
	}

	void execute()
	{
		std::map<size_t, std::string> p = _message->getParams();

		std::vector<std::string> _ch_params = split(p[0], ",");
		std::vector<std::string> _pw_params = split(p[1], ",");

		for (size_t i = 0; i < _ch_params.size(); i++)
		{
			Channel *channel = _server->getChannel(_ch_params[i]);

			if (channel)
			{
				if ((channel->_normal_clients.size() + channel->_ope_clients.size()) >
				    channel->_maxClients)
					_sender->message(
					    ERR_CHANNELISFULL(_sender->_servername, _sender->_nick, _ch_params[i]));
				else if (_pw_params.size() > 0)
				{
					if (channel->getPassword() == _pw_params[i])
					{
						channel->_normal_clients.push_back(_sender);
						_sender->message(std::string("Client " + _sender->_nick + " joined channel " + _ch_params[i] + "\n")
						                     .c_str());
					}
					else
						_sender->message(
						    ERR_BADCHANNELKEY(_sender->_servername, _sender->_nick)); // ERR_BADCHANNELKEY (475)
				}
				else
				{
					if (channel->getPassword() != "")
						_sender->message(
						    ERR_BADCHANNELKEY(_sender->_servername, _sender->_nick)); // ERR_BADCHANNELKEY (475)
					else
					{
						channel->_normal_clients.push_back(_sender);
						std::vector<std::string> users;
						std::string              users_str = "";
						for (size_t i = 0; i < channel->_normal_clients.size(); i++)
							users.push_back(channel->_normal_clients[i]->_nick);
						for (size_t i = 0; i < channel->_ope_clients.size(); i++)
							users.push_back(channel->_ope_clients[i]->_nick);
						for (size_t i = 0; i < users.size(); i++)
						{
							if (i == users.size() - 1)
								users_str += users[i];
							else
								users_str += users[i] + " ";
						}
						for (size_t j = 0; j < channel->_normal_clients.size(); j++)
						{
							if (_sender->_nick != channel->_normal_clients[j]->_nick)
							{
								channel->_normal_clients[i]->message(
								    RPL_NAMREPLY(_sender->_servername,
								                 channel->_normal_clients[j]->_nick, _ch_params[i], users_str));
								channel->_normal_clients[i]->message(
								    RPL_ENDOFNAMES(_sender->_servername,
								                   channel->_normal_clients[j]->_nick, _ch_params[i]));
							}
						}
						for (size_t j = 0; j < channel->_ope_clients.size(); j++)
						{
							if (_sender->_nick != channel->_ope_clients[j]->_nick)
							{
								channel->_ope_clients[i]->message(
								    RPL_NAMREPLY(_sender->_servername,
								                 channel->_ope_clients[j]->_nick, _ch_params[i], users_str));
								channel->_ope_clients[i]->message(
								    RPL_ENDOFNAMES(_sender->_servername,
								                   channel->_ope_clients[j]->_nick, _ch_params[i]));
							}
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
				        _sender->message(_sender->_nick + " " + _ch_params[i] + " :You have joined too many channels\n"); //ERR_TOOMANYCHANNELS (405)
				        return;
				}
				else */
				if (_pw_params.size() > 0)
				{
					channel = _server->createChannel(_ch_params[i], _pw_params[i]);
					_sender->message(std::string("Channel " + _ch_params[i] + " with password \"" + _pw_params[i] + "\" created\n")
					                     .c_str());
				}
				else
				{
					std::string pwd = std::string("");
					channel = _server->createChannel(_ch_params[i], pwd);
				}

				channel->setCreator(_sender);
				// :marc459!m@127.0.0.1 JOIN :#channel
				// RPL_TOPIC (332) marc459 #channel :No topic is set
				// RPL_TOPICWHOTIME (333)
				// RPL_NAMREPLY (353) 353 marc459 = #channel :@marc459
				// RPL_ENDOFNAMES (366) 366 marc459 #channel :End of /NAMES list.

				_sender->message(std::string(":" + _sender->_nick + "!" +
				                             _sender->_username + "@" + _sender->_servername + " JOIN :" + _ch_params[i] + "\n")
				                     .c_str());
				_sender->message(RPL_TOPIC(_sender->_servername, _sender->_nick, _ch_params[i], "No topic is set"));
				std::map<std::string, Command *>::iterator it;
				channel->_ope_clients.push_back(_sender);
				if ((it = _server->_commands.find("names")) !=
				    _server->_commands.end())
				{
					Command *   cmd = it->second;
					std::string names = "names";
					Message *   message = new Message(names);
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
