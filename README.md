## USAGE

## SERVER

make

./ircserv `<port>` `<password>`

## CLIENT

telnet `<serverip>` `<port>`

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


## Numeric Replies ##

RPL_WELCOME (001)
- Message: "<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
- Usecase: After client registration
RPL_YOURHOST (002)
- Message:
- Usecase:
RPL_CREATED (003)

RPL_MYINFO (004)

RPL_ISUPPORT (005)

RPL_BOUNCE (010)

RPL_UMODEIS (221)

RPL_LUSERCLIENT (251)

RPL_LUSEROP (252)

RPL_LUSERUNKNOWN (253)

RPL_LUSERCHANNELS (254)

RPL_LUSERME (255)

RPL_ADMINME (256)


RPL_ADMINLOC1 (257)

RPL_ADMINLOC2 (258)


RPL_ADMINEMAIL (259)

RPL_TRYAGAIN (263)

RPL_LOCALUSERS (265)

RPL_GLOBALUSERS (266)

RPL_WHOISCERTFP (276)

RPL_NONE (300)

RPL_AWAY (301)

RPL_USERHOST (302)

RPL_UNAWAY (305)

RPL_NOWAWAY (306)

RPL_WHOREPLY (352)

RPL_ENDOFWHO (315)

RPL_WHOISREGNICK (307)

RPL_WHOISUSER (311)

RPL_WHOISSERVER (312)

RPL_WHOISOPERATOR (313)

RPL_WHOWASUSER (314)


RPL_WHOISIDLE (317)
  "<client> <nick> <secs> <signon> :seconds idle, signon time"
Sent as a reply to the WHOIS command, this numeric indicates how long the client with the nickname <nick> has been idle. <secs> is the number of seconds since the client has been active. Servers generally denote specific commands (for instance, perhaps JOIN, PRIVMSG, NOTICE, etc) as updating the ‘idle time’, and calculate this off when the idle time was last updated. <signon> is a unix timestamp representing when the user joined the network. The text used in the last param of this message may vary.

RPL_ENDOFWHOIS (318)
  "<client> <nick> :End of /WHOIS list"
Sent as a reply to the WHOIS command, this numeric indicates the end of a WHOIS response for the client with the nickname <nick>.

<nick> MUST be exactly the <nick> parameter sent by the client in its WHOIS message. This means the case MUST be preserved, and if the client sent multiple nicks, this MUST be the comma-separated list of nicks, even if some of them were dropped.

This numeric is sent after all other WHOIS response numerics have been sent to the client.

RPL_WHOISCHANNELS (319)
  "<client> <nick> :[prefix]<channel>{ [prefix]<channel>}
Sent as a reply to the WHOIS command, this numeric lists the channels that the client with the nickname <nick> is joined to and their status in these channels. <prefix> is the highest channel membership prefix that the client has in that channel, if the client has one. <channel> is the name of a channel that the client is joined to. The last parameter of this numeric is a list of [prefix]<channel> pairs, delimited by a SPACE character (' ', 0x20).

RPL_WHOISCHANNELS can be sent multiple times in the same whois reply, if the target is on too many channels to fit in a single message.

The channels in this response are affected by the secret channel mode and the invisible user mode, and may be affected by other modes depending on server software and configuration.

RPL_WHOISSPECIAL (320)
  "<client> <nick> :blah blah blah"
Sent as a reply to the WHOIS command, this numeric is used for extra human-readable information on the client with nickname <nick>. This should only be used for non-essential information that does not need to be machine-readable or understood by client software.

RPL_LISTSTART (321)
  "<client> Channel :Users  Name"
Sent as a reply to the LIST command, this numeric marks the start of a channel list. As noted in the command description, this numeric MAY be skipped by the server so clients MUST NOT depend on receiving it.

RPL_LIST (322)
  "<client> <channel> <client count> :<topic>"
Sent as a reply to the LIST command, this numeric sends information about a channel to the client. <channel> is the name of the channel. <client count> is an integer indicating how many clients are joined to that channel. <topic> is the channel’s topic (as set by the TOPIC command).

RPL_LISTEND (323)
  "<client> :End of /LIST"
Sent as a reply to the LIST command, this numeric indicates the end of a LIST response.

RPL_CHANNELMODEIS (324)
  "<client> <channel> <modestring> <mode arguments>..."
Sent to a client to inform them of the currently-set modes of a channel. <channel> is the name of the channel. <modestring> and <mode arguments> are a mode string and the mode arguments (delimited as separate parameters) as defined in the MODE message description.

RPL_CREATIONTIME (329)
  "<client> <channel> <creationtime>"
Sent to a client to inform them of the creation time of a channel. <channel> is the name of the channel. <creationtime> is a unix timestamp representing when the channel was created on the network.

RPL_WHOISACCOUNT (330)
  "<client> <nick> <account> :is logged in as"
Sent as a reply to the WHOIS command, this numeric indicates that the client with the nickname <nick> was authenticated as the owner of <account>.

This does not necessarily mean the user owns their current nickname, which is covered byRPL_WHOISREGNICK.

RPL_NOTOPIC (331)
  "<client> <channel> :No topic is set"
Sent to a client when joining a channel to inform them that the channel with the name <channel> does not have any topic set.

RPL_TOPIC (332)
  "<client> <channel> :<topic>"
Sent to a client when joining the <channel> to inform them of the current topic of the channel.

RPL_TOPICWHOTIME (333)
  "<client> <channel> <nick> <setat>"
Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).

RPL_WHOISACTUALLY (338)
  "<client> <nick> :is actually ..."
  "<client> <nick> <host|ip> :Is actually using host"
  "<client> <nick> <username>@<hostname> <ip> :Is actually using host"
Sent as a reply to the WHOIS and WHOWAS commands, this numeric shows details about the client with the nickname <nick>.

<username> represents the name set by the USER command (though <username> may be set by the server in other ways).

<host> and <ip> represent the real host and IP address the client is connecting from. <host> CANNOT start with a colon (':', 0x3A) as this would get parsed as a trailing parameter – IPv6 addresses such as "::1" are prefixed with a zero ('0', 0x30) to ensure this. The resulting IPv6 is equivalent, as this is a partial expansion of the :: shorthand.

See also: RPL_WHOISHOST (378), for similar semantics on other servers.

RPL_INVITING (341)
  "<client> <nick> <channel>"
Sent as a reply to the INVITE command to indicate that the attempt was successful and the client with the nickname <nick> has been invited to <channel>.

RPL_INVITELIST (346)
  "<client> <channel> <mask>"
Sent as a reply to the MODE command, when clients are viewing the current entries on a channel’s invite-exception list. <mask> is the given mask on the invite-exception list.

RPL_ENDOFINVITELIST (347)
  "<client> <channel> :End of channel invite list"
Sent as a reply to the MODE command, this numeric indicates the end of a channel’s invite-exception list.

RPL_EXCEPTLIST (348)
  "<client> <channel> <mask>"
Sent as a reply to the MODE command, when clients are viewing the current entries on a channel’s exception list. <mask> is the given mask on the exception list.

RPL_ENDOFEXCEPTLIST (349)
  "<client> <channel> :End of channel exception list"
Sent as a reply to the MODE command, this numeric indicates the end of a channel’s exception list.

RPL_VERSION (351)
  "<client> <version> <server> :<comments>"
Sent as a reply to the VERSION command, this numeric indicates information about the desired server. <version> is the name and version of the software being used (including any revision information). <server> is the name of the server. <comments> may contain any further comments or details about the specific version of the server.

RPL_NAMREPLY (353)
  "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.

<symbol> notes the status of the channel. It can be one of the following:

("=", 0x3D) - Public channel.
("@", 0x40) - Secret channel (secret channel mode "+s").
("*", 0x2A) - Private channel (was "+p", no longer widely used today).
<nick> is the nickname of a client joined to that channel, and <prefix> is the highest channel membership prefix that client has in the channel, if they have one. The last parameter of this numeric is a list of [prefix]<nick> pairs, delimited by a SPACE character (' ', 0x20).

RPL_ENDOFNAMES (366)
  "<client> <channel> :End of /NAMES list"
Sent as a reply to the NAMES command, this numeric specifies the end of a list of channel member names.

RPL_BANLIST (367)
  "<client> <channel> <mask> [<who> <set-ts>]"
Sent as a reply to the MODE command, when clients are viewing the current entries on a channel’s ban list. <mask> is the given mask on the ban list.

<who> and <set-ts> are optional and MAY be included in responses. <who> is either the nickname or nickmask of the client that set the ban, or a server name, and <set-ts> is the UNIX timestamp of when the ban was set.

RPL_ENDOFBANLIST (368)
  "<client> <channel> :End of channel ban list"
Sent as a reply to the MODE command, this numeric indicates the end of a channel’s ban list.

RPL_ENDOFWHOWAS (369)
  "<client> <nick> :End of WHOWAS"
Sent as a reply to the WHOWAS command, this numeric indicates the end of a WHOWAS reponse for the nickname <nick>. This numeric is sent after all other WHOWAS response numerics have been sent to the client.

RPL_INFO (371)
  "<client> :<string>"
Sent as a reply to the INFO command, this numeric returns human-readable information describing the server: e.g. its version, list of authors and contributors, and any other miscellaneous information which may be considered to be relevant.

RPL_ENDOFINFO (374)
  "<client> :End of INFO list"
Indicates the end of an INFO response.

RPL_MOTDSTART (375)
  "<client> :- <server> Message of the day - "
Indicates the start of the Message of the Day to the client. The text used in the last param of this message may vary, and SHOULD be displayed as-is by IRC clients to their users.

RPL_MOTD (372)
  "<client> :<line of the motd>"
When sending the Message of the Day to the client, servers reply with each line of the MOTD as this numeric. MOTD lines MAY be wrapped to 80 characters by the server.

RPL_ENDOFMOTD (376)
  "<client> :End of /MOTD command."
Indicates the end of the Message of the Day to the client. The text used in the last param of this message may vary.

RPL_WHOISHOST (378)
  "<client> <nick> :is connecting from *@localhost 127.0.0.1"
Sent as a reply to the WHOIS command, this numeric shows details about where the client with nickname <nick> is connecting from.

See also: RPL_WHOISACTUALLY (338), for similar semantics on other servers.

RPL_WHOISMODES (379)
  "<client> <nick> :is using modes +ailosw"
Sent as a reply to the WHOIS command, this numeric shows the client what user modes the target users has.

RPL_YOUREOPER (381)
  "<client> :You are now an IRC operator"
Sent to a client which has just successfully issued an OPER command and gained operator status. The text used in the last param of this message varies wildly.

RPL_REHASHING (382)
  "<client> <config file> :Rehashing"
Sent to an operator which has just successfully issued a REHASH command. The text used in the last param of this message may vary.

RPL_TIME (391)
  "<client> <server> [<timestamp> [<TS offset>]] :<human-readable time>"
Reply to the TIME command. Typically only contains the human-readable time, but it may include a UNIX timestamp.

Clients SHOULD NOT parse the human-readable time.

<TS offset> is used by some servers using a TS-based server-to-server protocol (eg. TS6), and represents the offset between the server’s system time, and the TS of the network. A positive value means the server is lagging behind the TS of the network. Clients SHOULD ignore its value.

ERR_UNKNOWNERROR (400)
  "<client> <command>{ <subcommand>} :<info>"
Indicates that the given command/subcommand could not be processed. <subcommand> may repeat for more specific subcommands.

For example, for an issue with a hypothetical command PACK, this may be returned:

  :example.com 400 dan!~d@n PACK :Could not process multiple invalid parameters
For an issue with a hypothetical command PACK with the subcommand BOX, this may be returned:

  :example.com 400 dan!~d@n PACK BOX :Could not find box to pack
This numeric indicates a very generalised error (which <info> should further explain). If there is another more specific numeric which represents the error occuring, that should be used instead.

ERR_NOSUCHNICK (401)
  "<client> <nickname> :No such nick/channel"
Indicates that no client can be found for the supplied nickname. The text used in the last param of this message may vary.

ERR_NOSUCHSERVER (402)
  "<client> <server name> :No such server"
Indicates that the given server name does not exist. The text used in the last param of this message may vary.

ERR_NOSUCHCHANNEL (403)
  "<client> <channel> :No such channel"
Indicates that no channel can be found for the supplied channel name. The text used in the last param of this message may vary.

ERR_CANNOTSENDTOCHAN (404)
  "<client> <channel> :Cannot send to channel"
Indicates that the PRIVMSG / NOTICE could not be delivered to <channel>. The text used in the last param of this message may vary.

This is generally sent in response to channel modes, such as a channel being moderated and the client not having permission to speak on the channel, or not being joined to a channel with the no external messages mode set.

ERR_TOOMANYCHANNELS (405)
  "<client> <channel> :You have joined too many channels"
Indicates that the JOIN command failed because the client has joined their maximum number of channels. The text used in the last param of this message may vary.

ERR_WASNOSUCHNICK (406)
  "<client> :There was no such nickname"
Returned as a reply to WHOWAS to indicate there is no history information for that nickname.

ERR_NOORIGIN (409)
  "<client> :No origin specified"
Indicates a PING or PONG message missing the originator parameter which is required by old IRC servers. Nowadays, this may be used by some servers when the PING <token> is empty.

ERR_INPUTTOOLONG (417)
  "<client> :Input line was too long"
Indicates a given line does not follow the specified size limits (512 bytes for the main section, 4094 or 8191 bytes for the tag section).

ERR_UNKNOWNCOMMAND (421)
  "<client> <command> :Unknown command"
Sent to a registered client to indicate that the command they sent isn’t known by the server. The text used in the last param of this message may vary.

ERR_NOMOTD (422)
  "<client> :MOTD File is missing"
Indicates that the Message of the Day file does not exist or could not be found. The text used in the last param of this message may vary.

ERR_ERRONEUSNICKNAME (432)
  "<client> <nick> :Erroneus nickname"
Returned when a NICK command cannot be successfully completed as the desired nickname contains characters that are disallowed by the server. See the wire format section for more information on characters which are allowed in various IRC servers. The text used in the last param of this message may vary.

ERR_NICKNAMEINUSE (433)
  "<client> <nick> :Nickname is already in use"
Returned when a NICK command cannot be successfully completed as the desired nickname is already in use on the network. The text used in the last param of this message may vary.

ERR_USERNOTINCHANNEL (441)
  "<client> <nick> <channel> :They aren't on that channel"
Returned when a client tries to perform a channel+nick affecting command, when the nick isn’t joined to the channel (for example, MODE #channel +o nick).

ERR_NOTONCHANNEL (442)
  "<client> <channel> :You're not on that channel"
Returned when a client tries to perform a channel-affecting command on a channel which the client isn’t a part of.

ERR_USERONCHANNEL (443)
  "<client> <nick> <channel> :is already on channel"
Returned when a client tries to invite <nick> to a channel they’re already joined to.

ERR_NOTREGISTERED (451)
  "<client> :You have not registered"
Returned when a client command cannot be parsed as they are not yet registered. Servers offer only a limited subset of commands until clients are properly registered to the server. The text used in the last param of this message may vary.

ERR_NEEDMOREPARAMS (461)
  "<client> <command> :Not enough parameters"
Returned when a client command cannot be parsed because not enough parameters were supplied. The text used in the last param of this message may vary.

ERR_ALREADYREGISTERED (462)
  "<client> :You may not reregister"
Returned when a client tries to change a detail that can only be set during registration (such as resending the PASS or USER after registration). The text used in the last param of this message varies.

ERR_PASSWDMISMATCH (464)
  "<client> :Password incorrect"
Returned to indicate that the connection could not be registered as the password was either incorrect or not supplied. The text used in the last param of this message may vary.

ERR_YOUREBANNEDCREEP (465)
  "<client> :You are banned from this server."
Returned to indicate that the server has been configured to explicitly deny connections from this client. The text used in the last param of this message varies wildly and typically also contains the reason for the ban and/or ban details, and SHOULD be displayed as-is by IRC clients to their users.

ERR_CHANNELISFULL (471)
  "<client> <channel> :Cannot join channel (+l)"
Returned to indicate that a JOIN command failed because the client limit mode has been set and the maximum number of users are already joined to the channel. The text used in the last param of this message may vary.

ERR_UNKNOWNMODE (472)
  "<client> <modechar> :is unknown mode char to me"
Indicates that a mode character used by a client is not recognized by the server. The text used in the last param of this message may vary.

ERR_INVITEONLYCHAN (473)
  "<client> <channel> :Cannot join channel (+i)"
Returned to indicate that a JOIN command failed because the channel is set to [invite-only] mode and the client has not been invited to the channel or had an invite exemption set for them. The text used in the last param of this message may vary.

ERR_BANNEDFROMCHAN (474)
  "<client> <channel> :Cannot join channel (+b)"
Returned to indicate that a JOIN command failed because the client has been banned from the channel and has not had a ban exemption set for them. The text used in the last param of this message may vary.

ERR_BADCHANNELKEY (475)
  "<client> <channel> :Cannot join channel (+k)"
Returned to indicate that a JOIN command failed because the channel requires a key and the key was either incorrect or not supplied. The text used in the last param of this message may vary.

Not to be confused with ERR_INVALIDKEY, which may be returned when setting a key.

ERR_BADCHANMASK (476)
  "<channel> :Bad Channel Mask"
Indicates the supplied channel name is not a valid.

This is similar to, but stronger than, ERR_NOSUCHCHANNEL (403), which indicates that the channel does not exist, but that it may be a valid name.

The text used in the last param of this message may vary.

ERR_NOPRIVILEGES (481)
  "<client> :Permission Denied- You're not an IRC operator"
Indicates that the command failed because the user is not an IRC operator. The text used in the last param of this message may vary.

ERR_CHANOPRIVSNEEDED (482)
  "<client> <channel> :You're not channel operator"
Indicates that a command failed because the client does not have the appropriate channel privileges. This numeric can apply for different prefixes such as halfop, operator, etc. The text used in the last param of this message may vary.

ERR_CANTKILLSERVER (483)
  "<client> :You cant kill a server!"
Indicates that a KILL command failed because the user tried to kill a server. The text used in the last param of this message may vary.

ERR_NOOPERHOST (491)
  "<client> :No O-lines for your host"
Indicates that an OPER command failed because the server has not been configured to allow connections from this client’s host to become an operator. The text used in the last param of this message may vary.

ERR_UMODEUNKNOWNFLAG (501)
  "<client> :Unknown MODE flag"
Indicates that a MODE command affecting a user contained a MODE letter that was not recognized. The text used in the last param of this message may vary.

ERR_USERSDONTMATCH (502)
  "<client> :Cant change mode for other users"
Indicates that a MODE command affecting a user failed because they were trying to set or view modes for other users. The text used in the last param of this message varies, for instance when trying to view modes for another user, a server may send: "Can't view modes for other users".

ERR_HELPNOTFOUND (524)
  "<client> <subject> :No help available on this topic"
Indicates that a HELP command requested help on a subject the server does not know about.

The <subject> MUST be the one requested by the client, but may be casefolded; unless it would be an invalid parameter, in which case it MUST be *.

ERR_INVALIDKEY (525)
"<client> <target chan> :Key is not well-formed"
Indicates the value of a key channel mode change (+k) was rejected.

Not to be confused with ERR_BADCHANNELKEY, which is returned when someone tries to join a channel.

RPL_STARTTLS (670)
  "<client> :STARTTLS successful, proceed with TLS handshake"
This numeric is used by the IRCv3 tls extension and indicates that the client may begin a TLS handshake. For more information on this numeric, see the linked IRCv3 specification.

The text used in the last param of this message varies wildly.

RPL_WHOISSECURE (671)
  "<client> <nick> :is using a secure connection"
Sent as a reply to the WHOIS command, this numeric shows the client is connecting to the server in a way the server considers reasonably safe from eavesdropping (e.g. connecting from localhost, using TLS, using Tor).

ERR_STARTTLS (691)
  "<client> :STARTTLS failed (Wrong moon phase)"
This numeric is used by the IRCv3 tls extension and indicates that a server-side error occured and the STARTTLS command failed. For more information on this numeric, see the linked IRCv3 specification.

The text used in the last param of this message varies wildly.

ERR_INVALIDMODEPARAM (696)
"<client> <target chan/user> <mode char> <parameter> :<description>"
Indicates that there was a problem with a mode parameter. Replaces various implementation-specific mode-specific numerics.

RPL_HELPSTART (704)
"<client> <subject> :<first line of help section>"
Indicates the start of a reply to a HELP command. The text used in the last parameter of this message may vary, and SHOULD be displayed as-is by IRC clients to their users; possibly emphasized as the title of the help section.

The <subject> MUST be the one requested by the client, but may be casefolded; unless it would be an invalid parameter, in which case it MUST be *.

RPL_HELPTXT (705)
"<client> <subject> :<line of help text>"
Returns a line of HELP text to the client. Lines MAY be wrapped to a certain line length by the server. Note that the final line MUST be a RPL_ENDOFHELP (706) numeric.

The <subject> MUST be the one requested by the client, but may be casefolded; unless it would be an invalid parameter, in which case it MUST be *.

RPL_ENDOFHELP (706)
"<client> <subject> :<last line of help text>"
Returns the final HELP line to the client.

The <subject> MUST be the one requested by the client, but may be casefolded; unless it would be an invalid parameter, in which case it MUST be *.

ERR_NOPRIVS (723)
  "<client> <priv> :Insufficient oper privileges."
Sent by a server to alert an IRC operator that they they do not have the specific operator privilege required by this server/network to perform the command or action they requested. The text used in the last param of this message may vary.

<priv> is a string that has meaning in the server software, and allows an operator the privileges to perform certain commands or actions. These strings are server-defined and may refer to one or multiple commands or actions that may be performed by IRC operators.

Examples of the sorts of privilege strings used by server software today include: kline, dline, unkline, kill, kill:remote, die, remoteban, connect, connect:remote, rehash.

RPL_LOGGEDIN (900)
  "<client> <nick>!<user>@<host> <account> :You are now logged in as <username>"
This numeric indicates that the client was logged into the specified account (whether by SASL authentication or otherwise). For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

RPL_LOGGEDOUT (901)
  "<client> <nick>!<user>@<host> :You are now logged out"
This numeric indicates that the client was logged out of their account. For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

ERR_NICKLOCKED (902)
  "<client> :You must use a nick assigned to you"
This numeric indicates that SASL authentication failed because the account is currently locked out, held, or otherwise administratively made unavailable. For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

RPL_SASLSUCCESS (903)
  "<client> :SASL authentication successful"
This numeric indicates that SASL authentication was completed successfully, and is normally sent along with RPL_LOGGEDIN (900). For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

ERR_SASLFAIL (904)
  "<client> :SASL authentication failed"
This numeric indicates that SASL authentication failed because of invalid credentials or other errors not explicitly mentioned by other numerics. For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

ERR_SASLTOOLONG (905)
  "<client> :SASL message too long"
This numeric indicates that SASL authentication failed because the AUTHENTICATE command sent by the client was too long (i.e. the parameter was longer than 400 bytes). For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

ERR_SASLABORTED (906)
  "<client> :SASL authentication aborted"
This numeric indicates that SASL authentication failed because the client sent an AUTHENTICATE command with the parameter ('*', 0x2A). For more information on this numeric, see the IRCv3 sasl-3.1 extension.

The text used in the last param of this message varies wildly.

ERR_SASLALREADY (907)
  "<client> :You have already authenticated using SASL"
This numeric indicates that SASL authentication failed because the client has already authenticated using SASL and reauthentication is not available or has been administratively disabled. For more information on this numeric, see the IRCv3 sasl-3.1 and sasl-3.2 extensions.

The text used in the last param of this message varies wildly.

RPL_SASLMECHS (908)
  "<client> <mechanisms> :are available SASL mechanisms"
This numeric specifies the mechanisms supported for SASL authentication. <mechanisms> is a list of SASL mechanisms, delimited by a comma (',', 0x2C). For more information on this numeric, see the IRCv3 sasl-3.1 extension.

IRCv3.2 also specifies this information in the sasl client capability value. For more information on this, see the IRCv3 sasl-3.2 extension.

The text used in the last param of this message varies wildly.
