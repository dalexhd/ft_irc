#ifndef CLIENT_H
#define CLIENT_H

// Main libraries
#include <algorithm>
#include <fstream>
#include <netdb.h>
#include <unistd.h>

// Our includes
#include "./Message.hpp"
#include "./Replies.hpp"
#include "./config.hpp"

enum UserMode
{
	USER_MODE_INVISIBLE = 0,
	USER_MODE_OPERATOR = 1,
	USER_MODE_REGISTERED = 2,
};

class Client
{
  public:
	int         _fd;
	std::string _nick;
	std::string _username;
	std::string _realname;
	std::string _host;
	std::string _servername;

	std::string           _serverversion;
	std::vector<Message>  _messagesSent;
	std::vector<Message>  _messagesReceived;
	bool                  _is_authenticated;
	bool                  _is_passLogged;
	bool                  _is_ope;
	int const             _maxChannels;
	int                   _attempts;
	ClientStatus          _status;
	Message *             _message;
	std::vector<UserMode> _modes;

  public:
	Client(int &fd, std::string host, std::string servername, std::string version)
	    : _fd(fd), _host(host), _servername(servername), _serverversion(version), _is_authenticated(false), _is_passLogged(false), _is_ope(false), _maxChannels(MAX_CHANNELS), _attempts(0), _status(ALIVE){
	                                                                                                                                                                                             // message(RPL_WELCOME(_servername, _nick));
	                                                                                                                                                                                         };
	~Client()
	{
		close(_fd);
		_fd = -1;
		std::cout << C_RED << "Client " << _nick << " disconnected" C_X << std::endl;
	};

	// Helpers
	void message(char const *message);
	void message(std::string const message);
	void read(void);
	void disconnect(void)
	{
		delete this;
	}
	void ban(void)
	{
		message(ERR_YOUREBANNEDCREEP(_servername, _nick));
		disconnect();
	}
	std::vector<std::string> readConfigFile(char const *filename)
	{
		std::vector<std::string> motd;
		std::ifstream            data;
		data.open(filename);
		if (data.is_open())
		{
			std::string line;
			while (std::getline(data, line))
			{
				motd.push_back(line);
			}
			data.close();
		}
		return motd;
	}

	std::string getSupportString(void)
	{
		std::string              support = "";
		std::vector<std::string> motd = readConfigFile("./config/support");

		for (std::vector<std::string>::iterator it = motd.begin(); it != motd.end(); ++it)
		{
			support += *it + " ";
		}
		return support.substr(0, support.size() - 1);
	}

	void authenticate(void)
	{
		_is_authenticated = true;
		message(RPL_WELCOME(_servername, _nick));
		message(RPL_YOURHOST(_servername, _nick, _serverversion));
		message(RPL_CREATED(_servername, _nick));
		message(RPL_MYINFO(_servername, _nick, _serverversion, _servername, _serverversion));
		message(RPL_ISUPPORT(_servername, _nick, getSupportString()));
		message(RPL_MOTDSTART(_servername, _nick));
		std::vector<std::string> motd = readConfigFile("./config/motd");
		for (std::vector<std::string>::iterator it = motd.begin(); it != motd.end(); ++it)
		{
			message(RPL_MOTD(_servername, _nick, *it));
		}
		message(RPL_ENDOFMOTD(_servername, _nick));
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

	std::string const getUsername(void) const
	{
		return _username;
	};

	std::string const getRealname(void) const
	{
		return _realname;
	};

	std::string const getHost(void) const
	{
		return _host;
	};

	std::string const getUserId(void) const
	{
		return _nick + "!" + _username + "@" + _host;
	};

	std::string const getServername(void) const
	{
		return _servername;
	};

	bool isInvisible(void)
	{
		return this->hasMode(USER_MODE_INVISIBLE);
	};

	bool isOperator(void)
	{
		return this->hasMode(USER_MODE_OPERATOR);
	};

	bool isRegistered(void)
	{
		return this->hasMode(USER_MODE_REGISTERED);
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

	// Utils
	bool hasMode(UserMode mode)
	{
		return std::find(this->_modes.begin(), this->_modes.end(), mode) !=
		       this->_modes.end();
	}

	void addMode(UserMode mode)
	{
		if (!this->hasMode(mode))
			this->_modes.push_back(mode);
	}
};

#endif
