// Create IRC client in cpp
#include "Colors.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <unistd.h>
#include <vector>

#include <chrono>
#include <thread>

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
		for (size_t i = 5; i < this->_params.size(); i++)
		{
			command += " " + _params[i];
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
	std::string               _pass;
	int                       _socket;
	addrinfo                  hints, *servinfo;
	std::string               _username;
	std::string               _realname;
	std::map<size_t, Command> _commands;
	bool                      _connected;
	pthread_t                 _thread;
	std::string               _filename;

  public:
	Client(std::string host, std::string port)
	{
		this->_host = host;
		this->_port = port;
	}

	Client(std::string name, std::string username, std::string realname, std::string host = "127.0.0.1")
	{
		this->_name = name;
		this->_host = host;
		this->_port = "6667";
		this->_pass = "";
		this->_username = username;
		this->_realname = realname;
		this->_connected = false;
	}
	Client(const Client &copy)
	{
		this->_name = copy._name;
		this->_host = copy._host;
		this->_port = copy._port;
		this->_username = copy._username;
		this->_realname = copy._realname;
		this->_connected = copy._connected;
	}

	~Client()
	{
		close(this->_socket);
		// freeaddrinfo(servinfo);
	}

  public:
	int irc_connect(void)
	{
		int status;
		std::memset(&hints, 0, sizeof(addrinfo));

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_CANONNAME;

		try
		{
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
				throw std::runtime_error(this->_name + " Error connecting to "
				                                       "server");
			}
			usleep(500);
		}
		catch (std::exception &e)
		{
			std::cout << ROJO_T << std::endl << e.what() << RESET << std::endl;
			return (1);
		}
		return (0);
	}

	void irc_disconnect(void)
	{
		// this->send("QUIT");
		freeaddrinfo(servinfo);
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
		memset(buffer, 0, sizeof(buffer)); // initialize buffer to all zeros

		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(this->_socket, &readfds);

		struct timeval timeout;
		timeout.tv_sec = 4; // Wait up to 5 seconds for data to arrive
		timeout.tv_usec = 0;

		int ready = select(this->_socket + 1, &readfds, NULL, NULL, &timeout);
		if (ready < 0)
		{
			// Handle error
			return "";
		}
		else if (ready == 0)
		{
			// No data received within the timeout period
			return "";
		}

		// Data is available to be read
		if (read(this->_socket, buffer, sizeof(buffer)) <= 0)
		{
			// Handle error
			return "";
		}

		std::string tmp(buffer);
		return tmp;
	}

	void addCommand(Command command)
	{
		this->_commands.insert(std::pair<size_t, Command>(command.ms(), command));
	}

	friend std::ostream &operator<<(std::ostream &stream, const Client &client)
	{
		stream << "Client Name: " << client._name << std::endl
		       << "Username: " << client._username << std::endl
		       << "Realname: " << client._realname << std::endl
		       << std::endl;

		// SHOW CLIENT COMMANDS
		/*for(size_t i = 0;i < client._commands.size(); i++)
		{
		     stream << client._commands.at(i)._name << std::endl << std::endl;
		}*/
		return (stream);
	}

	std::string trim(std::string str)
	{
		std::size_t first = str.find_first_not_of(' ');
		if (first == std::string::npos)
		{
			return "";
		}
		std::size_t last = str.find_last_not_of(' ');
		return str.substr(first, last - first + 1);
	}

	int pingpong(std::string res)
	{
		std::cout << CYAN_T << res << RESET << std::endl;

		if ((std::strstr(res.c_str(), "PING")))
		{
			std::string pongArg = trim(res.substr(4, res.find('\n')));
			// std::cout << MAGENTA_T << pongArg << RESET << std::endl;
			send("PONG " + pongArg);
			return (0);
		}
		if (!_connected && (std::strstr(res.c_str(), "001")))
		{
			std::cout << MAGENTA_T << "CLIENT CONNECTED" << RESET << std::endl;
			this->_connected = true;
			return (0);
		}
		return (1);
	}

	std::vector<std::string> split(const std::string &str, const std::string &delimiters)
	{
		std::vector<std::string> tokens;

		std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			lastPos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, lastPos);
		}
		return tokens;
	}

	void login()
	{
		std::cout << "CLIENT CONNECTS" << std::endl; // IF not exists STACKOVERFLOW
		if (this->_pass != "")
		{
			send("PASS " + this->_pass); // PASS <server_password>
			usleep(1000);
		}
		send("NICK " + this->_name); // NICK <nickname>
		std::string serverresp = reads();
		int         i = 0;
		while (std::string::npos != split(serverresp, ":")[0].find("433"))
		{
			send("NICK " + this->_name + std::to_string(i));
			usleep(50000);
			serverresp = reads();
			i++;
		}
		usleep(1000);
		send("USER " + this->_username + " 0 * : " + this->_name + " " + this->_realname); // USER TestBot 0 * : msantos- surname
		this->_connected = true;
		pingpong(reads());
		usleep(1000);
	}
	void requestingLoop()
	{
		std::string line;

		std::thread t1([&]() {
			while (1)
				std::cout << reads();
		});
		t1.detach();
		for (line = ""; line != "quit" && std::getline(std::cin, line);)
		{
			send(line);
			usleep(1000);
			std::cout << reads();
		}
	}
};
