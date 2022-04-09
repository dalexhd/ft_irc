/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/04/09 19:49:32 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <cstdlib>
#include <fcntl.h>
#include <functional>

// Our includes
#include "./client.hpp"
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
	std::string const port;
	std::string const password;
	// Commands
	std::map<std::string, Command *> _commands;

  public:
	// Clients
	std::vector<Client *> _clients;

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

  private:
	void createServerListener(void);
	void createServerPoll(void);
	void removeClientFromServer(size_t clientId);
	int  readClient(size_t &i);
	void setupCommands(void);
};

#endif
