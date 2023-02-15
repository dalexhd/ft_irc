// Create IRC client in cpp
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

class Client
{
  public:
	std::string _name;
	std::string _host;
	std::string _port;
	int         _socket;
	addrinfo    hints, *servinfo;

  public:
	Client(std::string host, std::string port)
	{
		this->_host = host;
		this->_port = port;
		this->irc_connect();
	}

	~Client()
	{
		close(this->_socket);
		freeaddrinfo(servinfo);
	}

  private:
	void irc_connect(void)
	{
		int status;
		std::memset(&hints, 0, sizeof(addrinfo));

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_CANONNAME;

		// We get the server address.
		if (getaddrinfo(this->_host.c_str(), this->_port.c_str(), &hints, &servinfo) != 0)
			throw std::runtime_error("error: getaddrinfo");

		// We create the socket.
		if ((this->_socket =
		         socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
			throw std::runtime_error("Error while creating socket");
		status = connect(this->_socket, servinfo->ai_addr, servinfo->ai_addrlen);
		if (status == -1)
		{
			close(this->_socket);
			freeaddrinfo(servinfo);
			throw std::runtime_error("Error connecting to server");
		}
	}

  public:
	int send(std::string message)
	{
		std::string msg = message.append("\r\n");
		return write(this->_socket, msg.c_str(), msg.length());
	}

	std::string read(void)
	{
		char buffer[1024];

		while (!std::strstr(buffer, "\n"))
		{
			memset(buffer, '\0', sizeof(buffer));
			if (recv(this->_socket, buffer, sizeof(buffer), 0) <= 0)
				break;
		}
		buffer[strlen(buffer)] = '\0';
		std::string tmp(buffer);
		return (tmp);
	}

	void login(std::string name)
	{
		send("NICK " + name); // NICK msantos-
		usleep(1000);
		send("USER TestBot 0 * : " + name + " Surname"); // USER TestBot 0 * : msantos- Surname
		usleep(1000);
		std::cout << read() << std::endl;
	}
	void requestingLoop()
	{
		for (std::string line; line != "quit" && std::getline(std::cin, line);)
		{
			send(line);
			usleep(1000);
			std::cout << read() << std::endl;
		}
	}
};
