/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2023/03/23 18:29:43 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

// Main libraries
#include <fcntl.h>
#include <poll.h>

// Our includes
#include "./Channel.hpp"
#include "./Client.hpp"
#include "./Color.hpp"
#include "./config.hpp"

// Commands
class Command;

// Validation
bool validate_args(int argc, char **argv);

class Server
{
  private:
	typedef std::map<size_t, Client *>::iterator       clientMap_iterator;
	typedef std::map<std::string, Channel *>::iterator channelMap_iterator;
	typedef std::vector<pollfd>::iterator              pollVector_iterator;

	// Connection params
	std::string const host;
	std::string const servername;
	std::string const version;
	std::string const port;
	std::string const password;

  public:
	// Clients
	std::map<size_t, Client *> _clients;

	// Commands
	std::map<std::string, Command *> _commands;

	// Channels
	std::map<std::string, Channel *> _channels;

	// Socket status
	enum Status
	{
		OFFLINE,
		ONLINE,
		CLOSED
	} _status;

	// Socket specific vars
	std::vector<pollfd> _pfds;
	int                 _fd;

  public:
	Server(std::string host, std::string port, std::string password);
	~Server();
	void close_server();
	void run();

	// Helpers
	bool is_running(void)
	{
		return (_status == ONLINE);
	}
	bool hasPassword(void)
	{
		return (password != "");
	}
	std::string const getPassword(void)
	{
		return (password);
	}

	std::string const getServerName(void)
	{
		return (servername);
	}

	std::vector<Client *> getRelatedClients(Client *client)
	{
		std::vector<Channel *> channels = this->getChannels();
		std::vector<Client *>  related_clients;

		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i]->joined(client))
			{
				std::vector<Client *> channel_clients = channels[i]->getClients();
				for (size_t u = 0; u < channel_clients.size(); u++)
				{
					if (channel_clients[u] != client &&
					    !std::count(related_clients.begin(), related_clients.end(), channel_clients[u]))
						related_clients.push_back(channel_clients[u]);
				}
			}
		}
		return (related_clients);
	}

	std::map<std::string, Channel *> getRelatedChannels(Client *client)
	{
		std::map<std::string, Channel *> related_channels;

		std::map<std::string, Channel *>::iterator it = _channels.begin();
		for (; it != _channels.end(); it++)
		{
			if (it->second->joined(client))
			{
				related_channels.insert(std::pair<std::string, Channel *>(it->first, it->second));
			}
		}
		return (related_channels);
	}

  private:
	void createServerListener(void);
	void createServerPoll(void);
	void removeClientFromServer(size_t clientId);
	int  readClient(size_t &i);
	void setupCommands(void);

  public:
	// --------------
	// Clients stuff
	// --------------
	Client *getClient(std::string const &name)
	{
		std::map<size_t, Client *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++)
		{
			if (it->second->_nick == name)
			{
				return (it->second);
			}
		}
		return (NULL);
	}

	void deleteClient(int fd)
	{
		for (clientMap_iterator it = _clients.begin(); it != _clients.end();)
		{
			if (it->second->_fd == fd)
			{
				for (channelMap_iterator itc = _channels.begin();
				     itc != _channels.end();)
				{
					itc->second->removeClientFromChannel(it->second);
					if (itc->second->getClients().size() == 0)
					{
						delete itc->second;
						_channels.erase(itc++);
					}
					else
						itc++;
				}
				delete it->second;
				_clients.erase(it++);
				for (pollVector_iterator it2 = _pfds.begin(); it2 != _pfds.end();)
				{
					if (it2->fd == fd)
					{
						_pfds.erase(it2++);
						break;
					}
					else
						it2++;
				}
			}
			else
				it++;
		}
	}

	size_t getClientIndex(std::string &name)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->_nick == name)
				return (i);
		}
		return (-1);
	}

	// --------------
	// Channel stuff
	// --------------
	Channel *getChannel(std::string &name)
	{
		if (name.at(0) == '#')
			name = name.substr(1);
		if (_channels.find(name) != _channels.end())
			return _channels[name];
		else
			return NULL;
	}

	std::vector<Channel *> getChannels(void)
	{
		std::vector<Channel *>                     channels;
		std::map<std::string, Channel *>::iterator it;
		for (it = _channels.begin(); it != _channels.end(); it++)
			channels.push_back(it->second);
		return channels;
	}

	Channel *createChannel(std::string &name, std::string &password)
	{
		if (name.at(0) == '#')
			name = name.substr(1);
		_channels[name] = new Channel(name, password);
		return _channels[name];
	}

	// --------------
	// Commands stuff
	// --------------
	Command *getCommand(std::string &name)
	{
		if (_commands.find(name) != _commands.end())
			return _commands[name];
		return (NULL);
	}
};

#endif
