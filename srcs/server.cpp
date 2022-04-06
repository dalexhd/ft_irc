/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/06 18:50:27 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

/**
 * @brief Here we create the server object and we start the server listener.
 * If there is an error, we throw an exception.
 */
void Server::createServerListener()
{
	int      yes = 1;
	addrinfo hints = {.ai_flags = AI_PASSIVE, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM}, *servinfo;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// We get the server address.
	if (getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo) != 0)
		throw std::runtime_error("error: getaddrinfo" + std::string(strerror(errno)));

	// We create the socket.
	if ((_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
		throw std::runtime_error("error: socket" + std::string(strerror(errno)));
	else if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close(_socket);
		freeaddrinfo(servinfo);
		throw std::runtime_error("error: setsockopt" + std::string(strerror(errno)));
	}
	else if (bind(_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(_socket);
		freeaddrinfo(servinfo);
		throw std::runtime_error("error: bind" + std::string(strerror(errno)));
	}
	freeaddrinfo(servinfo);
	if (listen(_socket, MAX_CONNECTIONS) == -1) // Make listen socket
		throw std::runtime_error("error: listen");
	std::cout << "Server is listening on port " << port << std::endl;
}

int Server::readClient(size_t &i)
{
	char    buffer[MAX_BUFFER_SIZE];
	int     nbytes;
	pollfd &pfd = _pfds[i];

	Client *client = get_client(i);

	if ((nbytes = recv(pfd.fd, buffer, MAX_BUFFER_SIZE, 0)) <= 0)
	{
		if (nbytes == 0)
			std::cout << "Client " << client->name << " disconnected" << std::endl;
		else
			std::cout << "error: recv" << std::endl;
		return (-1);
	}
	buffer[nbytes - 1] = '\0';
	std::string message(buffer, sizeof(nbytes - 1));
	std::cout << "Received message from " << client->name << ": " << message << std::endl;
	if (message == "exit")
	{
		std::cout << "Client " << client->name << " disconnected" << std::endl;
		client->disconnect();
		return (-1);
	}
	else if (message == "ping")
	{
		std::cout << "Client " << client->name << " pinged" << std::endl;
		std::string res = "pong\n";
		client->message(res);
	}
	return (0);
}

void Server::createServerPoll(void)
{
	_pfds.push_back({.fd = _socket, .events = POLLIN, .revents = 0});
	while (is_running())
	{
		if (poll(_pfds.data(), _pfds.size(), -1) == -1)
			throw std::runtime_error("error: poll" + std::string(strerror(errno)));
		for (size_t i = 0; i < _pfds.size(); ++i)
		{
			if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _socket)
				{
					int new_fd;
					if ((new_fd = accept(_socket, NULL, NULL)) == -1)
						throw std::runtime_error("error: accept" + std::string(strerror(errno)));
					Client *client = new Client(_pfds[i], new_fd,
					                            "user" + std::to_string(_pfds.size()));
					clients.push_back(client);
					_pfds.push_back({.fd = new_fd, .events = POLLIN, .revents = 0});
					std::cout << "Client connected" << std::endl;
				}
				else
				{
					if (readClient(i) == -1)
						_pfds.erase(_pfds.begin() + i);
				}
			}
			if (_pfds[i].revents & POLLOUT)
			{
				std::cout << "Client disconnected" << std::endl;
				_pfds.erase(_pfds.begin() + i);
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
	createServerPoll();
}

void Server::run()
{
	int i = 0;
	while (this->is_running())
	{
		std::cout << "Server running" << std::endl;
		if (i == 5)
		{
			this->close_server();
			break;
		}
		sleep(1);
		i++;
	}
}

void Server::close_server()
{
	_status = CLOSED;
	std::cout << "Closing server..." << std::endl;
	delete this;
}

Server::~Server(void)
{
	std::cout << "Server closed" << std::endl;
}
