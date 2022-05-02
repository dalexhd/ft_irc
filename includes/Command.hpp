#ifndef COMMAND_H
#define COMMAND_H

// Our includes
#include "./server.hpp"

class Command
{
	// We make it protected in order to be able to implemet it
  protected:
	std::string                   _name;
	std::string                   _description;
	std::string                   _usage;
	bool                          _is_ope;
	std::map<size_t, std::string> _example;
	int                           _min_params;
	int                           _max_params;

	// Command non related vars
	Client * _sender;
	Server * _server;
	Message *_message;
	size_t   _sender_index;

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
	std::map<size_t, std::string> getExample() const
	{
		return _example;
	};
	Client * getSender() const
	{
		return _sender;

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
	void setMessage(Message *message)
	{
		_message = message;
	};
	bool hasOpe(void)
	{
		return (_is_ope);
	};
	int getMinParams()
	{
		return (_min_params);
	}
	int getMaxParams()
	{
		return (_max_params);
	}

  public:
	virtual void execute() = 0;
	Command() : _is_ope(false) , _min_params(0) ,_max_params(0) {};
	void         missingOpe(void)
	{
		_sender->message(
		    std::string("You need operator role in order to exec " + _name + "\n")
		        .c_str());
	};
	virtual ~Command()
	{
		std::cout << "Command " << _name << " destructor called" << std::endl;
	};
};

#endif
