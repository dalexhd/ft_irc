#ifndef COMMAND_H
#define COMMAND_H

// Our includes
#include "./Server.hpp"

class Command
{
	// We make it protected in order to be able to implemet it
  protected:
	std::string                   _name;
	std::string                   _description;
	std::string                   _usage;
	bool                          _is_ope;
	bool                          _needs_auth;
	std::map<size_t, std::string> _example;

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
	Client *getSender() const
	{
		return _sender;
	};
	void setSender(Client *sender)
	{
		_sender = sender;
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
	bool needsAuth(void)
	{
		return (_needs_auth);
	};

  public:
	virtual void execute() = 0;

	virtual bool validate(void)
	{
		return (true);
	}

	virtual std::vector<Message> parser(Message *message)
	{
		std::vector<Message> messages;
		messages.push_back(Message(message->_buffer));
		return (messages);
	}
	Command()
	    : _is_ope(false), _needs_auth(true), _sender(NULL), _server(NULL), _message(NULL){};
	void missingOpe(void)
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
