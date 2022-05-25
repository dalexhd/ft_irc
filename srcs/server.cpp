/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/05/25 15:39:47 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

#include "../includes/commands/Ban.hpp"
#include "../includes/commands/Echo.hpp"
#include "../includes/commands/Exit.hpp"
#include "../includes/commands/Help.hpp"
#include "../includes/commands/Info.hpp"
#include "../includes/commands/Name.hpp"
#include "../includes/commands/Ope.hpp"
#include "../includes/commands/Ping.hpp"

#include "../includes/cmds/Join.hpp"
#include "../includes/cmds/List.hpp"
#include "../includes/cmds/PrivMsg.hpp"

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
					_clients.push_back(
					    new Client(new_fd, std::string("user" + itoa(_clients.size() + 1)), this->host));
					pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
					_pfds.push_back(pfd);
					std::cout << "Client connected" << std::endl;
				}
				else if (i > 0)
				{
					Message *message = _clients[i - 1]->read();
					std::map<std::string, Command *>::iterator it;
					if ((it = _commands.find(message->getCmd())) != _commands.end())
					{
						Command *cmd = it->second;
						cmd->setSender(_clients[i - 1], i - 1);
						cmd->setServer(this);
						cmd->setMessage(message);
						if (!cmd->hasOpe() || (cmd->hasOpe() && _clients[i - 1]->_is_ope))
							cmd->execute();
						else
							cmd->missingOpe();
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
    : host(host), port(port), password(password), _status(ONLINE)
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
	_commands["name"] = new Name();

	_commands["privmsg"] = new PrivMsg();
	_commands["join"] = new Join();
	_commands["list"] = new List();
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

Command *Server::findCmd(std::string str)
{
	std::map<std::string, Command *>::iterator cmd = _commands.find(str);
	if (cmd != _commands.end())
		return (cmd->second);
	return (NULL);
}

Client *Server::findClient(std::string str)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (str.compare(_clients[i]->_name) == 0)
			return (_clients[i]);
	}
	return (NULL);
}
