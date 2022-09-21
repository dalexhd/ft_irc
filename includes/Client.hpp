#ifndef CLIENT_H
#define CLIENT_H

// Main libraries
#include <netdb.h>
#include <unistd.h>

// Our includes
#include "./Message.hpp"
#include "./Replies.hpp"
#include "./config.hpp"

class Client
{
  public:
	int                  _fd;
	std::string          _nick;
	std::string          _username;
	std::string          _realname;
	std::string          _host;
	std::string          _servername;
	std::string          _serverversion;
	std::vector<Message> _messagesSent;
	std::vector<Message> _messagesReceived;
	bool                 _is_authenticated;
	bool                 _is_ope;
	int const            _maxChannels;
	int                  _attempts;

  public:
	Client(int &fd, std::string host, std::string servername, std::string version)
	    : _fd(fd), _host(host), _servername(servername), _serverversion(version), _is_authenticated(false), _is_ope(false), _maxChannels(MAX_CHANNELS), _attempts(0){
	                                                                                                                                                        // message(RPL_WELCOME(_servername, _nick));
	                                                                                                                                                    };
	~Client()
	{
		close(_fd);
		_fd = -1;
		std::cout << "Client " << _nick << " disconnected" << std::endl;
	};

	// Helpers
	void     message(char const *message);
	void     message(std::string const message);
	Message *read(void);
	void     disconnect(void)
	{
		delete this;
	}
	void ban(void)
	{
		message(ERR_YOUREBANNEDCREEP(_servername, _nick));
		disconnect();
	}
	void authenticate(void)
	{
		_is_authenticated = true;
		message(RPL_WELCOME(_servername, _nick));
		message(RPL_YOURHOST(_servername, _nick, _serverversion));
		message(RPL_CREATED(_servername, _nick));
		// message(RPL_MYINFO(_servername, _nick, _serverversion, _servername, _serverversion));
		// TODO: todo lo demás
	};
	void loginFail(void)
	{
		_attempts++;
		if (_attempts >= MAX_ATTEMPTS)
			ban();
	};

	// Getters
	std::vector<Message> messagesSent() const
	{
		return _messagesSent;
	};

	std::vector<Message> messagesReceived() const
	{
		return _messagesReceived;
	};

	bool isAuthenticated(void) const
	{
		return _is_authenticated;
	};

	bool canPerformPassCommand(void) const
	{
		return (_nick != "" && _username != "" && _realname != "");
	};

	std::string const getNick(void) const
	{
		return _nick;
	};

	// Setters
	void setNick(std::string const &nick)
	{
		_nick = nick;
	};
	void setUsername(std::string const &username)
	{
		_username = username;
	};
	void setRealName(std::string const &realname)
	{
		_realname = realname;
	};
};

#endif
