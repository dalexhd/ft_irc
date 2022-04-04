/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:25:49 by aborboll          #+#    #+#             */
/*   Updated: 2022/03/30 18:11:14 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

/**
 * @brief Here we create the server object and we start the server listener.
 * If there is an error, we throw an exception.
 *
 * @param port The port to listen to
 * @param password The password to use
 */
Server::Server(char *port, char *password) : status(1)
{
	int opt;

	// Socket specific vars/options
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error("socket() failed" + std::string(strerror(errno)));
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("setsockopt() failed" + std::string(strerror(errno)));

	address.sin_family = AF_INET;
	address.sin_port = htons(std::atoi(port));
	address.sin_addr.s_addr = INADDR_ANY;

	// First we need to bind the socket to a port. If there is an error, we will throw an exception.
	if (bind(fd, (struct sockaddr *) &address, sizeof(address)) == -1)
		throw std::runtime_error("bind() failed " + std::string(strerror(errno)));
	// Now we need to listen for connections. Maxiumum of 3 connections.
	if (listen(fd, 3) == -1)
		throw std::runtime_error("listen() failed " + std::string(strerror(errno)));

	std::cout << "Server started at port " << port << " with password: " << password << std::endl
	          << "Waiting for incomming connections..." << std::endl;
}

void Server::run()
{
	int i = 0;
	while (this->is_running())
	{
		std::cout << "Server running" << std::endl;
		if (i == 5)
		{
			this->close();
			break;
		}
		sleep(1);
		i++;
	}
}

void Server::close()
{
	std::cout << "Closing server..." << std::endl;
	delete this;
}

Server::~Server(void)
{
	std::cout << "Server closed" << std::endl;
}
