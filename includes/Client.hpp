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
#include "./Replies.hpp"
#include "./config.hpp"

#include <fstream>

class Client
{
  public:
	int                  _fd;
	std::string          _name;
	std::string          _user;
	std::string          _host;
	std::vector<Message> _messagesSent;
	std::vector<Message> _messagesReceived;
	bool                 _is_ope;
	int const            _maxChannels;

  public:
	Client(int &fd, std::string name, std::string host)
	    : _fd(fd), _name(name), _host(host), _is_ope(false), _maxChannels(MAX_CHANNELS)
  {
		message(RPL_WELCOME(_host, _name));
	};
	~Client()
	{
		close(_fd);
		_fd = -1;
		std::cout << "Client " << _name << " disconnected" << std::endl;
	};

	// Helpers
	void     message(char const *message);
	void     message(std::string const message);
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
		this->message(
		    std::string("Your name has been changed to " + _name + "\n").c_str());
	};
};

#endif
