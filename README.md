## USAGE

## SERVER

make

./ircserv `<port>` `<password>`

## CLIENT

telnet `<serverip>` `<port>`


## COMMANDS DOCS ##

https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands

- pass -> Sets a connection password.This command must be sent before the NICK/USER registration combination

ex /nick Carlos
- privmsg ->  Sends message to msgtarget, which is usually a user or channel.

ex: "PRIVMSG msgtarget :message"

ex "/msg Ilene"
- ping -> Lets you determine the network distance between your computer and other users 

ex: "/ping Ilene"
- names -> Instructs the server to send names in an RPL_NAMES reply prefixed with all their respective channel statuses instead of just the highest one

- join -> Makes the client join the channels in the comma-separated list, specifying the passwords, if needed

- part -> Causes a user to leave the channels in the comma-separated list .

ex: "/part channel"
"
- topic -> Allows the client to query or set the channel topic on channel

ex: "/topic channel"

- list -> Lists all channels on the server

ex: "/list"

- invites -> Invites nickname to the channel channel

ex: "/invites nickname channel"

- kick -> Forcibly removes client from channel

ex: "/kick client channel"
