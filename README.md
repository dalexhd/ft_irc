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