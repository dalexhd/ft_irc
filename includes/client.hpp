#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <poll.h>
#include <string.h>
#include <string>
#include <unistd.h>

// Socket includes
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

// Our includes
#include "./config.hpp"

class Client
{
  private:
	// Client vars
	pollfd const _pfd;

  public:
	int         _fd;
	std::string name;
	size_t      id;

  public:
	Client(pollfd const &pfd, int &fd, std::string name)
	    : _pfd(pfd), _fd(fd), name(name){};
	~Client()
	{
		(void) _pfd;
		std::cout << "Client " << name << " destroyed!" << std::endl;
	};

	// Helpers
	void message(std::string &message);
	void disconnect(void);
};

#endif
