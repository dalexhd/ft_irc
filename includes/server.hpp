/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/06 17:45:57 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <map>
#include <poll.h>
#include <string.h>
#include <unistd.h>
#include <vector>

// Socket includes
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// Our includes
#include "./client.hpp"
#include "./config.hpp"

// Validation
bool validate_args(int argc, char **argv);

class Server
{
  private:
	// Connection params
	std::string const host;
	std::string const port;
	std::string const password;

	// Clients
	std::vector<Client *> clients;

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

	Client *get_client(size_t &id)
	{
		return clients[id - 1];
	}

  private:
	void createServerListener(void);
	void createServerPoll(void);
	int  readClient(size_t &i);
};

#endif
