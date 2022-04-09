#ifndef CLIENT_H
#define CLIENT_H

// Main libraries
#include <cstring>
#include <iostream>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <vector>

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

  public:
	Client(pollfd const &pfd, int &fd, std::string name)
	    : _pfd(pfd), _fd(fd), name(name){};
	~Client()
	{
		(void) _pfd;
		close(_fd);
		_fd = -1;
		std::cout << "Client " << name << " disconnected" << std::endl;
	};

	// Helpers
	void message(char const *message);
	int  read(void);
	void disconnect(void);
};

#endif
