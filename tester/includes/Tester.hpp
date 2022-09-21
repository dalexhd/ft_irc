// Create IRC client in cpp
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

class Tester
{
  public:
	std::string _dirname;

  public:
	Tester(std::string dirname)
	{
		this->_dirname = dirname;
		if (open(dirname.c_str(), O_RDWR | O_CREAT | O_RDWR, 0666) == -1)
			throw std::runtime_error("Error while creating file");
	}

  public:
	int send(std::string message)
	{
		std::string msg = message.append("\r\n");
		return write(this->_socket, msg.c_str(), msg.length());
	}
};
