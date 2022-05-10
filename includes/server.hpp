/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/05/10 16:48:31 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <cstdlib>
#include <fcntl.h>
#include <functional>

// Our includes
#include "./Color.hpp"
#include "./client.hpp"
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

};

#endif
