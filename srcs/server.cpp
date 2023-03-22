/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2023/03/16 17:40:37 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"

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
	if ((_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
		throw std::runtime_error("Error while creating socket");
	// We set the socket options.
	else if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close(_fd);
		freeaddrinfo(servinfo);
		throw std::runtime_error("Error while setting socket options");
	}
	else if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_fd);
		freeaddrinfo(servinfo);
		throw std::runtime_error("Error while setting socket to non-blocking");
	}
	// We bind the socket.
	else if (bind(_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(_fd);
		freeaddrinfo(servinfo);
		throw std::runtime_error("Error while binding socket");
	}
	freeaddrinfo(servinfo);
	// We listen for connections. We set the max connections to MAX_CONNECTIONS (defined in Server.hpp).
	if (listen(_fd, MAX_CONNECTIONS) == -1)
		throw std::runtime_error("Error while listening for connections");
	// We add the socket to the pollfd vector.
	pollfd pfd = {.fd = _fd, .events = POLLIN, .revents = 0};
	_pfds.push_back(pfd);
	std::cout << "Server is listening on port " << port << std::endl;
}

void Server::createServerPoll(void)
{
	while (is_running())
	{
		if (poll(_pfds.data(), _pfds.size(), -1) == -1)
			throw std::runtime_error("Error while polling for events");
		if (_pfds[0].revents & POLLIN)
		{
			int new_fd;
			if ((new_fd = accept(_fd, NULL, NULL)) == -1)
				throw std::runtime_error("error: accept");
			_clients[new_fd] = new Client(new_fd, this->host, this->servername, this->version);
			pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
			_pfds.push_back(pfd);
			std::cout << "Anonymous Client connected" << std::endl;
		}
		else
		{
			std::vector<pollfd>::iterator i = _pfds.begin();
			while (i != _pfds.end())
			{
				if (i->revents & POLLIN)
				{
					_clients[i->fd]->read();
					if (_clients[i->fd]->_status == DISCONNECTED)
					{
						deleteClient(_clients[i->fd]->_fd);
						break;
					}
					else
					{
						Message *message = _clients[i->fd]->_message;
						std::cout << "Message received " << message->_buffer << std::endl;
						std::map<std::string, Command *>::iterator it;
						if ((it = _commands.find(message->getCmd())) !=
						    _commands.end())
						{
							Command *cmd = it->second;
							cmd->setSender(_clients[i->fd]);
							cmd->setServer(this);
							cmd->setMessage(message);
							if (cmd->validate())
							{
								if (!cmd->needsAuth())
									cmd->execute();
								else if (_clients[i->fd]->isAuthenticated())
								{
									if (!cmd->hasOpe() ||
									    (cmd->hasOpe() && _clients[i->fd]->_is_ope))
										cmd->execute();
									else
										cmd->missingOpe();
								}
							}
							else
								std::cout << "Command not valid" << std::endl;
							break;
						}
						else if (message->getCmd() == "close")
						{
							_status = Status(CLOSED);
						}
					}
				}
				i++;
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
	_commands["help"] = new Help();
	_commands["ope"] = new Ope();
	_commands["quit"] = new Quit();
	_commands["privmsg"] = new PrivMsg();
	_commands["join"] = new Join();
	_commands["part"] = new Part();
	_commands["list"] = new List();
	_commands["names"] = new Names();
	_commands["kick"] = new Kick();
	_commands["notice"] = new Notice();
	_commands["nick"] = new Nick();
	_commands["user"] = new User();
	_commands["pass"] = new Pass();
	_commands["mode"] = new Mode();
	_commands["whois"] = new Whois();
	_commands["topic"] = new Topic();
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
