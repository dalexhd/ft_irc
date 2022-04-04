/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 17:13:29 by aborboll          #+#    #+#             */
/*   Updated: 2022/03/30 18:02:03 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <functional>
#include <iostream>
#include <string.h>
#include <unistd.h>

// Socket includes
#include <netinet/in.h>
#include <sys/socket.h>

// Validation
bool validate_args(int argc, char **argv);

class Server
{
  private:
	int status;

	// Socket specific vars
	int fd;
	// int         _socket;
	sockaddr_in address;

  public:
	Server(char *port, char *password);
	~Server();
	void close();
	void run();
	// Helpers
	bool is_running(void)
	{
		return (status == 1);
	}
};

#endif
