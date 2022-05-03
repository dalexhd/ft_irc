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
	std::string _name;
	std::vector<Message> _messagesSent;
	std::vector<Message> _messagesReceived;
	bool        _is_ope;

  public:
	Client(int &fd, std::string name) : _fd(fd), _name(name), _is_ope(false){};
	~Client()
	{
		close(_fd);
		_fd = -1;
		std::cout << "Client " << _name << " disconnected" << std::endl;
	};

	// Helpers
	void     message(char const *message);
	void Client::message(std::string const message);
	Message *read(void);
	void     disconnect(void);

	// Getters
	std::vector<Message> messagesSent() const
	{
		return _messagesSent;
	};

	std::vector<Message> messagesReceived() const
	{
		return _messagesReceived;
	};

	// Setters
	void setName(std::string const &name)
	{
		_name = name;
		this->message(std::string("Your name has been changed to " + _name + "\n").c_str());
	};
};

#endif
