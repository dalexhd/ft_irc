#ifndef MESSAGE_H
#define MESSAGE_H

// Our includes
#include "./Color.hpp"
#include "./functions.hpp"

#include <cstring>
#include <iostream>
#include <map>

class Message
{
  public:
	std::string _buffer;

  private:
	std::string                   _cmd;
	std::map<size_t, std::string> _params;

  public:
	Message(std::string &buffer);
	std::string                   getCmd(void) const;
	std::map<size_t, std::string> getParams(void) const;
};

#endif
