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
#include "./Message.hpp"
#include "./config.hpp"

class Client
{
  public:
	int         _fd;
	std::string name;

  public:
	Client(int &fd, std::string name) : _fd(fd), name(name){};
	~Client()
	{
		close(_fd);
		_fd = -1;
		std::cout << "Client " << name << " disconnected" << std::endl;
	};

	// Helpers
	void     message(char const *message);
	Message *read(void) const;
	void     disconnect(void);
};

#endif
