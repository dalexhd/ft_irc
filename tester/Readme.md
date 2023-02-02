# IRC TESTER #

Run several clients and execute commands from ./spec/* 

## GETTING STARTED

### USAGE

make

### TESTER EXPLANATION

IrcTester create multiple users that executes all commands presaved in ./spec/*
and comapares it result with the files ./res/*

### TESTS SYNTAXIS

Sentences must be preceded with some parameters

and user will be atutomaticly registered and logged in server.

After that, commands will be sent by the correct user in the required timestamp.

<span style="color:#4f6b5b"># timestamp, user nick, index, fullname</span>.


[0] [NICK_0] [USER_0] [REAL_NAME_0] [IRCCOMMAND]

