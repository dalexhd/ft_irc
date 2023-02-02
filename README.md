<div align="center">

![image](https://i.giphy.com/media/dbtDDSvWErdf2/giphy.webp)

## OUR OWN IRC SERVER

[![aborboll's 42 ft_irc Score](https://badge42.vercel.app/api/v2/cl2uk1olu003009mgf4ilrst7/project/2564130)](https://github.com/JaeSeoKim/badge42)

[![GitHub last commit](https://img.shields.io/github/last-commit/dalexhd/ft_irc?color=blue&label=Last%20commit&logo=git&maxAge=3600)](https://github.com/dalexhd/ft_irc/commits)
[![GitHub issues](https://img.shields.io/github/issues/dalexhd/ft_irc?label=Issues&color=blue&maxAge=3600)](https://github.com/dalexhd/ft_irc/issues)
[![GitHub forks](https://img.shields.io/github/forks/dalexhd/ft_irc?label=Forks&color=blue&maxAge=3600)](https://github.com/dalexhd/ft_irc/network)

[![Test](https://github.com/dalexhd/ft_irc/workflows/Test/badge.svg)](https://github.com/dalexhd/ft_irc/actions?query=workflow:Test)
[![codecov](https://codecov.io/gh/dalexhd/ft_irc/branch/main/graph/badge.svg?token=D8A1FV32VL)](https://codecov.io/gh/dalexhd/ft_irc)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/dalexhd/ft_irc?label=Code%20size&maxAge=3600)](https://github.com/dalexhd/ft_irc)

</div>


## SERVER

make

./ircserv `<port>` `<password>`

## CLIENT

telnet `<serverip>` `<port>`

 - The ircserver must be tested with Graphical Official Irc clients as limechat, weechat, textualchat

## COMMANDS DOCS

https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

- **pass** -> Sets a connection password.This command must be sent before the NICK/USER registration combination

      ex "pass <password>"

- **nick** -> Allows a client to change their IRC nickname

      ex "/nick <nickname>"

- **privmsg** -> Sends message to msgtarget, which is usually a user or channel.

      ex: "/privmsg msgtarget :message"

- **ping** -> Lets you determine the network distance between your computer and other users

      ex: "/ping <nickname>"

- **names** -> Returns a list of who is on the comma-separated list of <channels>, by channel name.

      ex: "/names <channel>"

- **join** -> Makes the client join the channels in the comma-separated list, specifying the passwords, if needed

      ex: "/join <channels> <keys>"

- **part** -> Causes a user to leave the channels in the comma-separated list .

      ex: "/part <channel>"

- **topic** -> Allows the client to query or set the channel topic on channel

      ex: "/topic <channel>"

- **list** -> Lists all channels on the server

      ex: "/list"

- **invites** -> Invites nickname to the channel channel

      ex: "/invites <nickname> <channel>"

- **kick** -> Forcibly removes client from channel

      ex: "/kick <client> <channel>"

## [](#message-format)Message Format

An IRC message is a single line, delimited by a pair of `CR` `('\r', 0x0D)` and `LF` `('\n', 0x0A)` characters.

- When reading messages from a stream, read the incoming data into a buffer. Only parse and process a message once you encounter the `\r\n` at the end of it. If you encounter an empty message, silently ignore it.
- When sending messages, ensure that a pair of `\r\n` characters follows every single message your software sends out.

---

Messages have this format, as rough ABNF:

      message         ::= ['@' <tags> SPACE] [':' <source> SPACE] <command> <parameters> <crlf>
      SPACE           ::=  %x20 *( %x20 )   ; space character(s)
      crlf            ::=  %x0D %x0A        ; "carriage return" "linefeed"

The specific parts of an IRC message are:

- **tags**: Optional metadata on a message, starting with `('@', 0x40)`.
- **source**: Optional note of where the message came from, starting with `(':', 0x3A)`.
- **command**: The specific command this message represents.
- **parameters**: If it exists, data relevant to this specific command.

These message parts, and parameters themselves, are separated by one or more ASCII SPACE characters `(' ', 0x20)`.

Most IRC servers limit messages to 512 bytes in length, including the trailing `CR-LF` characters. Implementations which include [message tags](https://ircv3.net/specs/extensions/message-tags.html) need to allow additional bytes for the **tags** section of a message; clients must allow 8191 additional bytes and servers must allow 4096 additional bytes.

---

## NUMERIC REPLIES

https://modern.ircdocs.horse/#numerics

RPL_WELCOME (001)

- Message: "-client- :Welcome to the -networkname- Network, -nick-[!<user>@<host>]"

- Usecase: After client registration

RPL_YOURHOST (002)

- Message: "-client- :Your host is -servername-, running version -version-"
- Usecase: Part of the post-registration greeting

RPL_CREATED (003)

- Message: "-client- :This server was created -datetime-"
- Usecase: Part of the post-registration greeting,

RPL_MYINFO (004)

- Message:
- Usecase:

RPL_ISUPPORT (005)

- Message:
- Usecase:

RPL_BOUNCE (010)

- Message:
- Usecase:

RPL_UMODEIS (221)

- Message:
- Usecase:

  RPL_LUSERCLIENT (251)

- Message:
- Usecase:

  RPL_LUSEROP (252)

- Message:
- Usecase:

  RPL_LUSERUNKNOWN (253)

- Message:
- Usecase:

  RPL_LUSERCHANNELS (254)

- Message:
- Usecase:

  RPL_LUSERME (255)

- Message:
- Usecase:

## ADMIN

RPL_ADMINME (256)

- Message:
- Usecase:

  RPL_ADMINLOC1 (257)

- Message:
- Usecase:

  RPL_ADMINLOC2 (258)

- Message:
- Usecase:

  RPL_ADMINEMAIL (259)

- Message:
- Usecase:

#

RPL_TRYAGAIN (263)

- Message: "-client- -command- :Please wait a while and try again."
- Usecase: When a server drops a command without processing it, this numeric MUST be sent to inform the client.

## LUSERS

RPL_LOCALUSERS (265)

- Message:
- Usecase:

  RPL_GLOBALUSERS (266)

- Message:
- Usecase:

#

RPL_WHOISCERTFP (276)

- Message:
- Usecase:

  RPL_NONE (300)

- Message:
- Usecase:

## AWAY

RPL_AWAY (301)

- Message:
- Usecase:

  RPL_USERHOST (302)

- Message:
- Usecase:

  RPL_UNAWAY (305)

- Message:
- Usecase:

## WHO

RPL_NOWAWAY (306)

- Message:
- Usecase:

  RPL_WHOREPLY (352)

- Message:
- Usecase:

  RPL_ENDOFWHO (315)

- Message:
- Usecase:

  RPL_WHOISREGNICK (307)

- Message:
- Usecase:

  RPL_WHOISUSER (311)

- Message:
- Usecase:

  RPL_WHOISSERVER (312)

- Message:
- Usecase:

  RPL_WHOISOPERATOR (313)

- Message:
- Usecase:

  RPL_WHOWASUSER (314)

- Message:
- Usecase:

  RPL_WHOISIDLE (317)

- Message:
- Usecase:

RPL_ENDOFWHOIS (318)

- Message:
- Usecase:

RPL_WHOISCHANNELS (319)

- Message:
- Usecase:

RPL_WHOISSPECIAL (320)

- Message:
- Usecase:

## LIST

RPL_LISTSTART (321)

- Message: "-client- Channel :Users Name"
- Usecase: this numeric marks the start of a channel list

RPL_LIST (322)

- Message: "-client- -channel- -client count- :-topic-"
- Usecase: Sent as a reply to the LIST command,information about a channel to the client. -channel- is the name of the channel. -client count- is an integer indicating how many clients are joined to that channel. -topic- is the channel’s topic

RPL_LISTEND (323)

- Message: "-client- :End of /LIST"
- Usecase: Sent as a reply to the LIST command, this numeric indicates the end of a LIST response.

#

RPL_CHANNELMODEIS (324)

- Message:
- Usecase:

  RPL_CREATIONTIME (329)

- Message:
- Usecase:

RPL_WHOISACCOUNT (330)

- Message:
- Usecase:

RPL_NOTOPIC (331)

- Message:
- Usecase:

RPL_TOPIC (332)

- Message:
- Usecase:

RPL_TOPICWHOTIME (333)

- Message:
- Usecase:

RPL_WHOISACTUALLY (338)

- Message:
- Usecase:

RPL_INVITING (341)

- Message:
- Usecase:

  RPL_INVITELIST (346)

- Message:
- Usecase:

  RPL_ENDOFINVITELIST (347)

- Message:
- Usecase:

RPL_EXCEPTLIST (348)

- Message:
- Usecase:

  RPL_ENDOFEXCEPTLIST (349)

- Message:
- Usecase:

RPL_VERSION (351)

- Message:
- Usecase:

RPL_NAMREPLY (353)

- Message:
- Usecase:

## NAMES

RPL_ENDOFNAMES (366)

- Message: "-client- -channel- :End of /NAMES list"
- Usecase: Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.

RPL_BANLIST (367)

- Message:
- Usecase:

RPL_ENDOFBANLIST (368)

- Message:
- Usecase:

RPL_ENDOFWHOWAS (369)

- Message:
- Usecase:

## INFO

RPL_INFO (371)

- Message: "-client- :-string-"
- Usecase: Sent as a reply to the INFO command, this numeric returns human-readable information describing the server: e.g. its version, list of authors and contributors, and any other miscellaneous information which may be considered to be relevant.

RPL_ENDOFINFO (374)

- Message: "-client- :End of INFO list"
- Usecase: Indicates the end of an INFO response.

RPL_MOTDSTART (375)

- Message:
- Usecase:

RPL_MOTD (372)

- Message:
- Usecase:

RPL_ENDOFMOTD (376)

- Message:
- Usecase:

RPL_WHOISHOST (378)

- Message:
- Usecase:

RPL_WHOISMODES (379)

- Message:
- Usecase:

RPL_YOUREOPER (381)

- Message:
- Usecase:

RPL_REHASHING (382)

- Message:
- Usecase:

RPL_TIME (391)

- Message:
- Usecase:

  ERR_UNKNOWNERROR (400)

- Message: "-client- -command-{ -subcommand-} :-info-"
- Usecase: Indicates that the given command/subcommand could not be processed

ERR_NOSUCHNICK (401)

- Message: "-client- -nickname- :No such nick/channel"
- Usecase: Indicates that no client can be found for the supplied nickname.

ERR_NOSUCHSERVER (402)

- Message: "-client- -server name- :No such server"
- Usecase: Indicates that the given server name does not exist.

ERR_NOSUCHCHANNEL (403)

- Message: "-client- -channel- :No such channel"
- Usecase: Indicates that no channel can be found for the supplied channel name.

ERR_CANNOTSENDTOCHAN (404)

- Message: "-client- -channel- :Cannot send to channel"
- Usecase: Indicates that the PRIVMSG / NOTICE could not be delivered to -channel-.

  ERR_TOOMANYCHANNELS (405)

- Message: "-client- -channel- :You have joined too many channels"
- Usecase: Indicates that the JOIN command failed because the client has joined their maximum number of channels.

ERR_WASNOSUCHNICK (406)

- Message: "-client- :There was no such nickname"
- Usecase: Returned as a reply to WHOWAS to indicate there is no history information for that nickname.

ERR_NOORIGIN (409)

- Message: "-client- :No origin specified"
- Usecase: Indicates a PING or PONG message missing the originator parameter which is required by old IRC servers.

ERR_INPUTTOOLONG (417)

- Message: "-client- :Input line was too long"
- Usecase: Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).

ERR_UNKNOWNCOMMAND (421)

- Message: "-client- -command- :Unknown command"
- Usecase: Sent to a registered client to indicate that the command they sent isn’t known by the server.

ERR_NOMOTD (422)

- Message: "-client- :MOTD File is missing"
- Usecase: Indicates that the Message of the Day file does not exist or could not be found.

ERR_ERRONEUSNICKNAME (432)

- Message:"-client- -nick- :Erroneus nickname"
- Usecase: Returned when a NICK command cannot be successfully completed as the desired nickname contains characters that are disallowed by the server.

ERR_NICKNAMEINUSE (433)

- Message: "-client- -nick- :Nickname is already in use"
- Usecase: Returned when a NICK command cannot be successfully completed as the desired nickname is already in use

ERR_USERNOTINCHANNEL (441)

- Message: "-client- -nick- -channel- :They aren't on that channel"
- Usecase: Returned when a client tries to perform a channel+nick affecting command, when the nick isn’t joined to the channel

ERR_NOTONCHANNEL (442)

- Message: "-client- -channel- :You're not on that channel"
- Usecase: Returned when a client tries to perform a channel-affecting command on a channel which the client isn’t a part of.

ERR_USERONCHANNEL (443)

- Message: "-client- -nick- -channel- :is already on channel"
- Usecase: Returned when a client tries to invite -nick- to a channel they’re already joined to.

ERR_NOTREGISTERED (451)

- Message: "-client- :You have not registered"
- Usecase: Returned when a client command cannot be parsed as they are not yet registered.

  ERR_NEEDMOREPARAMS (461)

- Message: "-client- -command- :Not enough parameters"
- Usecase: Returned when a client command cannot be parsed because not enough parameters were supplied.

ERR_ALREADYREGISTERED (462)

- Message: "-client- :You may not reregister"
- Usecase: Returned when a client tries to change a detail that can only be set during registration (such as resending the PASS or USER after registration).

ERR_PASSWDMISMATCH (464)

- Message: "-client- :Password incorrect"
- Usecase: Returned to indicate that the connection could not be registered as the password was either incorrect or not supplied.

ERR_YOUREBANNEDCREEP (465)

- Message: "-client- :You are banned from this server."
- Usecase: Returned to indicate that the server has been configured to explicitly deny connections from this client.

ERR_CHANNELISFULL (471)

- Message: "-client- -channel- :Cannot join channel (+l)"
- Usecase: Returned to indicate that a JOIN command failed because the client limit mode has been set and the maximum number of users are already joined to the channel.

ERR_UNKNOWNMODE (472)

- Message: "-client- -modechar- :is unknown mode char to me"
- Usecase: Indicates that a mode character used by a client is not recognized by the server. The text used in the last param of this message may vary.

ERR_INVITEONLYCHAN (473)

- Message: "-client- -channel- :Cannot join channel (+i)"
- Usecase: Returned to indicate that a JOIN command failed because the channel is set to [invite-only] mode and the client has not been invited to the channel or had an invite exemption set for them.

ERR_BANNEDFROMCHAN (474)

- Message: "-client- -channel- :Cannot join channel (+b)"
- Usecase: Returned to indicate that a JOIN command failed because the client has been banned from the channel and has not had a ban exemption set for them. The text used in the last param of this message may vary.

  ERR_BADCHANNELKEY (475)

- Message: "-client- -channel- :Cannot join channel (+k)"
- Usecase: Returned to indicate that a JOIN command failed because the channel requires a key and the key was either incorrect or not supplied.

ERR_BADCHANMASK (476)

- Message: "-channel- :Bad Channel Mask"
- Usecase: Indicates the supplied channel name is not a valid.This is similar to, but stronger than, ERR_NOSUCHCHANNEL (403), which indicates that the channel does not exist, but that it may be a valid name.

ERR_NOPRIVILEGES (481)

- Message: "-client- :Permission Denied- You're not an IRC operator"
- Usecase: Indicates that the command failed because the user is not an IRC operator.

ERR_CHANOPRIVSNEEDED (482)

- Message: "-client- -channel- :You're not channel operator"
- Usecase: Indicates that a command failed because the client does not have the appropriate channel privileges.

ERR_CANTKILLSERVER (483)

- Message: "-client- :You cant kill a server!"
- Usecase: Indicates that a KILL command failed because the user tried to kill a server.

ERR_NOOPERHOST (491)

- Message: "-client- :No O-lines for your host"
- Usecase: Indicates that an OPER command failed because the server has not been configured to allow connections from this client’s host to become an operator.

ERR_UMODEUNKNOWNFLAG (501)

- Message: "-client- :Unknown MODE flag"
- Usecase: Indicates that a MODE command affecting a user contained a MODE letter that was not recognized.

ERR_USERSDONTMATCH (502)

- Message: "-client- :Cant change mode for other users"
- Usecase: Indicates that a MODE command affecting a user failed because they were trying to set or view modes for other users.

ERR_HELPNOTFOUND (524)

- Message: "-client- -subject- :No help available on this topic"
- Usecase: Indicates that a HELP command requested help on a subject the server does not know about.

ERR_INVALIDKEY (525)

- Message: "-client- -target chan- :Key is not well-formed"
- Usecase: Indicates the value of a key channel mode change (+k) was rejected.

RPL_STARTTLS (670)

- Message: "-client- :STARTTLS successful, proceed with TLS handshake"
- Usecase: This numeric is used by the IRCv3 tls extension and indicates that the client may begin a TLS handshake.

RPL_WHOISSECURE (671)

- Message:
- Usecase:

ERR_STARTTLS (691)

- Message:
- Usecase:

ERR_INVALIDMODEPARAM (696)

- Message: "-client- -target chan // user- -mode char- -parameter- :-description-"
- Usecase: Indicates that there was a problem with a mode parameter. Replaces various implementation-specific mode-specific numerics.

RPL_HELPSTART (704)

- Message: "-client- -subject- :-first line of help section-"
- Usecase: Indicates the start of a reply to a HELP command.

RPL_HELPTXT (705)

- Message: "-client- -subject- :-line of help text-"
- Usecase: Returns a line of HELP text to the client. Lines MAY be wrapped to a certain line length by the server.

RPL_ENDOFHELP (706)

- Message: "-client- -subject- :-last line of help text-"
- Usecase: Returns the final HELP line to the client.

ERR_NOPRIVS (723)

- Message: "-client- -priv- :Insufficient oper privileges."
- Usecase: Sent by a server to alert an IRC operator that they they do not have the specific operator privilege required by this server/network to perform the command or action they requested.

RPL_LOGGEDIN (900)

- Message: "-client- -nick-!-user-@-host- -account- :You are now logged in as -username-"
- Usecase: This numeric indicates that the client was logged into the specified account

RPL_LOGGEDOUT (901)

- Message: "-client- -nick-!-user-@-host- :You are now logged out"
- Usecase: This numeric indicates that the client was logged out of their account.

ERR_NICKLOCKED (902)

- Message: "-client- :You must use a nick assigned to you"
- Usecase:

RPL_SASLSUCCESS (903)

- Message:
- Usecase:

ERR_SASLFAIL (904)

- Message:
- Usecase:

ERR_SASLTOOLONG (905)

- Message:
- Usecase:

ERR_SASLABORTED (906)

- Message:
- Usecase:

ERR_SASLALREADY (907)

- Message:
- Usecase:

RPL_SASLMECHS (908)

- Message:
- Usecase:
