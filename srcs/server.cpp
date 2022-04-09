/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/09 16:38:17 by aborboll         ###   ########.fr       */
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
	addrinfo hints, *servinfo;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

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

void Server::removeClientFromServer(size_t clientId)
{
	// First we delete the client pointer, this will execute the client
	// destructor which will close the socket. delete _clients[];
	delete _clients[clientId];
	// Then we remove the client from the clients map containers.
	_clients.erase(_clients.begin() + clientId);
	// Then we remove the client from the clients _pfds.
	_pfds.erase(_pfds.begin() + clientId + 1);
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
					_clients.push_back(new Client(_pfds[i], new_fd, std::string("user")));
					pollfd pfd = {.fd = new_fd, .events = POLLIN, .revents = 0};
					_pfds.push_back(pfd);
					std::cout << "Client connected" << std::endl;
				}
				else if (i > 0 && _clients[i - 1]->read() == -1)
					removeClientFromServer(i - 1);
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
