/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/06/24 15:05:36 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include "../includes/commands/Ban.hpp"
#include "../includes/commands/Echo.hpp"
#include "../includes/commands/Exit.hpp"
#include "../includes/commands/Help.hpp"
#include "../includes/commands/Info.hpp"
#include "../includes/commands/Ope.hpp"
#include "../includes/commands/Ping.hpp"

// CHANNEL FUNCTIONS

#include "../includes/cmds/Invite.hpp"
#include "../includes/cmds/Join.hpp"
#include "../includes/cmds/Kick.hpp"
#include "../includes/cmds/List.hpp"
#include "../includes/cmds/Mode.hpp"
#include "../includes/cmds/Names.hpp"
#include "../includes/cmds/Nick.hpp"
#include "../includes/cmds/Part.hpp"
#include "../includes/cmds/Pass.hpp"
#include "../includes/cmds/PrivMsg.hpp"
#include "../includes/cmds/Quit.hpp"
#include "../includes/cmds/User.hpp"

/**
 * @brief Here we create the server object and we start the server listener.
 * If there is an error, we throw an exception.
 */
void Server::createServerListener()
{
	int      yes = 1;
	addrinfo hints, *servinfo;
	std::memset(&hints, 0, sizeof(addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	// We get the server address.
	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) != 0)
		throw std::runtime_error("error: getaddrinfo");

	// We create the socket.
	if ((_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
		throw std::runtime_error("error: socket");
	else if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close(_socket);
		freeaddrinfo(servinfo);
		throw std::runtime_error("error: setsockopt");
	}
	else if (bind(_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(_socket);
		freeaddrinfo(servinfo);
		throw std::runtime_error("error: bind");
	}
	freeaddrinfo(servinfo);
	if (listen(_socket, MAX_CONNECTIONS) == -1) // Make listen socket
		throw std::runtime_error("error: listen");
	std::cout << "Server is listening on port " << port << std::endl;
}

void Server::createServerPoll(void)
{
	pollfd pfd = {.fd = _socket, .events = POLLIN, .revents = 0};

	_pfds.push_back(pfd);
	while (is_running())
	{
		if (poll(_pfds.data(), _pfds.size(), -1) == -1)
			throw std::runtime_error("error: poll");
		for (size_t i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _socket)
				{
					int new_fd;
					if ((new_fd = accept(_socket, NULL, NULL)) == -1)
						throw std::runtime_error("error: accept");
					_clients.push_back(new Client(new_fd, this->host, this->servername, this->version));
					pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
					_pfds.push_back(pfd);
					std::cout << "Anonymous Client connected" << std::endl;
				}
				else if (i > 0)
				{
					Message *message = _clients[i - 1]->read();
					std::cout << "Message received " << message->_buffer << std::endl;
					std::map<std::string, Command *>::iterator it;
					if ((it = _commands.find(message->getCmd())) != _commands.end())
					{
						Command *cmd = it->second;
						cmd->setSender(_clients[i - 1], i - 1);
						cmd->setServer(this);
						cmd->setMessage(message);
						if (cmd->validate())
						{
							if (!cmd->needsAuth())
								cmd->execute();
							else if (_clients[i - 1]->isAuthenticated())
							{
								if (!cmd->hasOpe() ||
								    (cmd->hasOpe() && _clients[i - 1]->_is_ope))
									cmd->execute();
								else
									cmd->missingOpe();
							}
						}
						break;
					}
					else if (message->getCmd() == "close")
					{
						_status = Status(CLOSED);
					}
				}
			}
		}
	}
}

/**
 * @brief Here we init the server instance
 *
 * @param port The port to listen to
 * @param password The password to use
 */
Server::Server(std::string host, std::string port, std::string password)
    : host(host), servername(SERVER_NAME), version(SERVER_VERSION), port(port), password(password), _status(ONLINE)
{
	createServerListener();
	setupCommands();
	createServerPoll();
}

void Server::setupCommands(void)
{
	_commands["ping"] = new Ping();
	_commands["info"] = new Info();
	_commands["exit"] = new Exit();
	_commands["echo"] = new Echo();
	_commands["help"] = new Help();
	_commands["ban"] = new Ban();
	_commands["ope"] = new Ope();
	_commands["quit"] = new Quit();

	// Channel commands
	_commands["privmsg"] = new PrivMsg();
	_commands["join"] = new Join();
	_commands["part"] = new Part();
	_commands["list"] = new List();
	_commands["names"] = new Names();
	_commands["kick"] = new Kick();
	_commands["nick"] = new Nick();
	_commands["user"] = new User();
	_commands["pass"] = new Pass();
	_commands["mode"] = new Mode();
}

Server::~Server(void)
{
	std::map<std::string, Command *>::iterator it = _commands.begin();
	// We delete all commands
	for (; it != _commands.end(); it++)
		delete it->second;

	// We delete all clients
	for (size_t i = 0; i < _clients.size(); i++)
		delete (_clients[i]);
	_clients.clear();

	// We delete all channels
	std::map<std::string, Channel *>::iterator it_ch = _channels.begin();
	for (; it_ch != _channels.end(); it_ch++)
		delete (it_ch->second);
	_channels.clear();
	std::cout << "Server closed!" << std::endl;
}
