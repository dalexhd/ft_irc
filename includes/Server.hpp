/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/06/23 17:49:46 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <cstdlib>
#include <fcntl.h>
#include <string>

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
	// Connection params
	std::string const host;
	std::string const servername;
	std::string const version;
	std::string const port;
	std::string const password;

  public:
	// Clients
	std::vector<Client *> _clients;

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
	int                 _socket;

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
					std::cout << channel_clients[u]->getNick() << std::endl;
					if (channel_clients[u] != client &&
					    !std::count(related_clients.begin(), related_clients.end(), channel_clients[u]))
						related_clients.push_back(channel_clients[u]);
				}
			}
		}
		return (related_clients);
	}

	std::vector<Channel *> getRelatedChannels(Client *client)
	{
		std::vector<Channel *> channels = this->getChannels();
		std::vector<Channel *> related_channels;

		for (size_t i = 0; i < channels.size(); i++)
		{
			if (channels[i]->joined(client))
			{
				related_channels.push_back(channels[i]);
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
		for (size_t i = 0; i < _clients.size(); i++)
		{
			std::cout << "Client name: " << _clients[i]->getNick() << std::endl;
			if (_clients[i]->getNick() == name)
				return (_clients[i]);
		}
		std::cout << "Client not found" << std::endl;
		return (NULL);
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
		return _channels[name];
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
