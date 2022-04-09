#ifndef COMMAND_H
#define COMMAND_H

// Our includes
#include "./server.hpp"

class Command
{
	// We make it protected in order to be able to implemet it
  protected:
	std::string _name;
	std::string _description;
	std::string _usage;
	std::string _example;
	// Command non related vars
	Client *_sender;
	Server *_server;
	size_t  _sender_index;

  public:
	std::string getName() const
	{
		return _name;
	};
	std::string getDescription() const
	{
		return _description;
	};
	std::string getUsage() const
	{
		return _usage;
	};
	std::string getExample() const
	{
		return _example;
	};
	void setSender(Client *sender, size_t index)
	{
		_sender = sender;
		_sender_index = index;
	};
	void setServer(Server *server)
	{
		_server = server;
	};

  public:
	virtual void execute() = 0;
	virtual ~Command()
	{
		std::cout << "Command " << _name << " destructor called" << std::endl;
	};
};

#endif
