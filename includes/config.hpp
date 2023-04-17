#ifndef CONFIG_H
#define CONFIG_H

#define MAX_CONNECTIONS 300
#define MAX_BUFFER_SIZE 512
#define MAX_CHANNELS 5
#define SERVER_NAME "42irc-server"
#define MAX_ATTEMPTS 5
#define SERVER_VERSION "0.0.1"
#define MAX_CLIENTS_PER_CHANNEL 255

enum ClientStatus
{
	ALIVE,
	DISCONNECTED
};

#endif
