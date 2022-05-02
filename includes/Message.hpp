#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstring>
#include <iostream>
#include <map>
#include <string>


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

/*std::ostream &operator <<(std::ostream &out, const Message &ms)
{
	std::map<size_t, std::string>::iterator it;
	for( it = ms.getParams().begin(); it != ms.getParams().end(); ++it)
	{
    	out << it->second << " ";
	}
	out << std::endl;
    return out;
}*/

#endif
