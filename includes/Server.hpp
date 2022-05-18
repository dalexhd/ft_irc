/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/05/18 18:38:16 by aborboll         ###   ########.fr       */
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
#include "./functions.hpp"

// Commands
class Command;

// Validation
bool validate_args(int argc, char **argv);

class Server
{
  private:
	// Connection params
	std::string const host;
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
	bool is_running(void){return (_status == ONLINE);}
	Command *findCmd(std::string str);
	Client *findClient(std::string str);

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
	Client *getClient(std::string &name)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->_name == name)
				return (_clients[i]);
		}
		return (NULL);
	}

	size_t getClientIndex(std::string &name)
	{
		for (size_t i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->_name == name)
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
};

#endif
