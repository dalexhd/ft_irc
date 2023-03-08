// Create IRC client in cpp
#include <cstring>
#include <iostream>
#include <map>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

class Command
{
  public:
	size_t                   _ms;
	std::string              _name;
	std::vector<std::string> _params;

  public:
	Command(size_t ms, std::string name, std::vector<std::string> params)
	{
		this->_ms = ms;
		this->_name = name;
		this->_params = params;
	}
	~Command()
	{
	}

	size_t ms()
	{
		return (this->_ms);
	}

	std::string name()
	{
		return (this->_name);
	}

	std::vector<std::string> params()
	{
		return (this->_params);
	}

	std::string getCommand()
	{
		std::string command = this->_name;
		for (std::vector<std::string>::const_iterator it = this->_params.begin();
		     it != this->_params.end(); ++it)
		{
			command += " " + *it;
		}
		return (command);
	}

  public:
	friend std::ostream &operator<<(std::ostream &stream, const Command &command)
	{
		stream << command._ms << " " << command._name << " ";
		for (std::vector<std::string>::const_iterator it = command._params.begin();
		     it != command._params.end(); ++it)
		{
			stream << *it << " ";
		}
		return (stream);
	}
};

class Client
{
  public:
	std::string               _name;
	std::string               _host;
	std::string               _port;
	int                       _socket;
	addrinfo                  hints, *servinfo;
	std::string               _username;
	std::string               _realname;
	std::map<size_t, Command> _commands;
	bool                      _connected;

  public:
	Client(std::string host, std::string port)
	{
		this->_host = host;
		this->_port = port;
	}

	Client(std::string name, std::string username, std::string realname)
	{
		this->_name = name;
		this->_host = "127.0.0.1";
		this->_port = "6667";
		this->_username = username;
		this->_realname = realname;
		this->_connected = false;
	}

	~Client()
	{
		close(this->_socket);
		// freeaddrinfo(servinfo);
	}

  public:
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

		usleep(1000);
		this->send("NICK " + this->_name);
		usleep(1000);
		this->send("USER " + this->_username + " 0 * :" + this->_realname);
		this->_connected = true;
		usleep(1000);
	}

	void irc_disconnect(void)
	{
		this->send("QUIT");
	}

  public:
	int send(std::string message)
	{
		std::string msg = message.append("\r\n");
		return write(this->_socket, msg.c_str(), msg.length());
	}

	std::string reads(void)
	{
		char buffer[1024];

		while (!std::strstr(buffer, "\n"))
		{
			memset(buffer, '\0', sizeof(buffer));
			if (read(this->_socket, buffer, sizeof(buffer)) <= 0)
				break;
		}
		buffer[strlen(buffer)] = '\0';
		std::string tmp(buffer);
		return (tmp);
	}

	void addCommand(Command command)
	{
		this->_commands.insert(std::pair<size_t, Command>(command.ms(), command));
	}

	friend std::ostream &operator<<(std::ostream &stream, const Client &client)
	{
		stream << "Client Name: " << client._name << std::endl
		       << "Username: " << client._username << std::endl
		       << "Realname: " << client._realname << std::endl;
		return (stream);
	}
};
