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
