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

#include <fstream>

class Client
{
  public:
	int                  _fd;
	std::string          _name;
	std::vector<Message> _messagesSent;
	std::vector<Message> _messagesReceived;
	bool                 _is_ope;
	int			const    _maxChannels;

  public:
	std::string itoa(int a)
	{
		std::string ss = ""; // create empty string
		while (a)
		{
			int x = a % 10;
			a /= 10;
			char i = '0';
			i = i + x;
			ss = i + ss; // append new character at the front of the string!
		}
		return ss;
	}

	Client(int &fd, std::string name) : _fd(fd), _name(name), _is_ope(false), _maxChannels(2)
	{
		std::string line;
		std::ifstream     myfile ("Welcome.txt", std::ifstream::in);
		if (myfile.is_open())
		{
			while ( getline (myfile,line) )
			{
				std::cout << line << '\n';
			}
			 myfile.close();
		}
		else std::cout << "Unable to open file";

	//	message("\n-​ +-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​+\n-​ |Welcome to FT_IRC Network!\n-​ +-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​-​+\n\nIf you are looking for assistance, please try:\nhelp\n\n");
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
