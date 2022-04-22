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

## NUMERIC REPLIES
https://modern.ircdocs.horse/#numerics

RPL_WELCOME (001)

- Message: "-client- :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"

- Usecase: After client registration

RPL_YOURHOST (002)

- Message: "-client- :Your host is <servername>, running version <version>"
- Usecase: Part of the post-registration greeting

RPL_CREATED (003)

- Message: "-client- :This server was created <datetime>"
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

- Message: "<client> <command> :Please wait a while and try again."
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

- Message:  "<client> Channel :Users  Name"
- Usecase: this numeric marks the start of a channel list

RPL_LIST (322)

- Message:  "<client> <channel> <client count> :<topic>"
- Usecase: Sent as a reply to the LIST command,information about a channel to the client. <channel> is the name of the channel. <client count> is an integer indicating how many clients are joined to that channel. <topic> is the channel’s topic

RPL_LISTEND (323)

- Message: "<client> :End of /LIST"
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

- Message: "<client> <channel> :End of /NAMES list"
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

- Message: "<client> :<string>"
- Usecase: Sent as a reply to the INFO command, this numeric returns human-readable information describing the server: e.g. its version, list of authors and contributors, and any other miscellaneous information which may be considered to be relevant.

RPL_ENDOFINFO (374)

- Message: "<client> :End of INFO list"
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

## ERRORS
  ERR_UNKNOWNERROR (400)
- Message:
- Usecase:

ERR_NOSUCHNICK (401)

- Message:
- Usecase:

ERR_NOSUCHSERVER (402)

- Message:
- Usecase:

ERR_NOSUCHCHANNEL (403)

- Message:
- Usecase:

ERR_CANNOTSENDTOCHAN (404)

- Message:
- Usecase:
  ERR_TOOMANYCHANNELS (405)
- Message:
- Usecase:

ERR_WASNOSUCHNICK (406)

- Message:
- Usecase:

ERR_NOORIGIN (409)

- Message:
- Usecase:

ERR_INPUTTOOLONG (417)

- Message:
- Usecase:

ERR_UNKNOWNCOMMAND (421)

- Message:
- Usecase:

ERR_NOMOTD (422)

- Message:
- Usecase:

ERR_ERRONEUSNICKNAME (432)

- Message:
- Usecase:

ERR_NICKNAMEINUSE (433)

- Message:
- Usecase:

ERR_USERNOTINCHANNEL (441)

- Message:
- Usecase:

ERR_NOTONCHANNEL (442)

- Message:
- Usecase:

ERR_USERONCHANNEL (443)

- Message:
- Usecase:

ERR_NOTREGISTERED (451)

- Message:
- Usecase:
  ERR_NEEDMOREPARAMS (461)
- Message:
- Usecase:

ERR_ALREADYREGISTERED (462)

- Message:
- Usecase:

ERR_PASSWDMISMATCH (464)

- Message:
- Usecase:

ERR_YOUREBANNEDCREEP (465)

- Message:
- Usecase:

ERR_CHANNELISFULL (471)

- Message:
- Usecase:

ERR_UNKNOWNMODE (472)

- Message:
- Usecase:

ERR_INVITEONLYCHAN (473)

- Message:
- Usecase:

ERR_BANNEDFROMCHAN (474)

- Message:
- Usecase:
  ERR_BADCHANNELKEY (475)
- Message:
- Usecase:

ERR_BADCHANMASK (476)

- Message:
- Usecase:

ERR_NOPRIVILEGES (481)

- Message:
- Usecase:

ERR_CHANOPRIVSNEEDED (482)

- Message:
- Usecase:

ERR_CANTKILLSERVER (483)

- Message:
- Usecase:

ERR_NOOPERHOST (491)

- Message:
- Usecase:

ERR_UMODEUNKNOWNFLAG (501)

- Message:
- Usecase:

ERR_USERSDONTMATCH (502)

- Message:
- Usecase:

ERR_HELPNOTFOUND (524)

- Message:
- Usecase:

ERR_INVALIDKEY (525)

- Message:
- Usecase:

RPL_STARTTLS (670)

- Message:
- Usecase:

RPL_WHOISSECURE (671)

- Message:
- Usecase:

ERR_STARTTLS (691)

- Message:
- Usecase:

ERR_INVALIDMODEPARAM (696)

- Message:
- Usecase:

RPL_HELPSTART (704)

- Message:
- Usecase:

RPL_HELPTXT (705)

- Message:
- Usecase:

RPL_ENDOFHELP (706)

- Message:
- Usecase:

ERR_NOPRIVS (723)

- Message:
- Usecase:

RPL_LOGGEDIN (900)

- Message:
- Usecase:

RPL_LOGGEDOUT (901)

- Message:
- Usecase:

ERR_NICKLOCKED (902)

- Message:
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
