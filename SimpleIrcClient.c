#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 512

struct addrinfo *servinfo;

void irc_free(int sockfd)
{
	freeaddrinfo(servinfo);
	if (close(sockfd) == -1)
		perror("connect close");
}

int irc_connect(char *host, char *port)
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(host, port, &hints, &servinfo);
	if (status == -1)
	{
		perror("addrinfo");
		return -1;
	}

	int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1)
	{
		freeaddrinfo(servinfo);
		perror("connect socket");
		return -1;
	}

	status = connect(sock, servinfo->ai_addr, servinfo->ai_addrlen);

	if (status == -1)
	{
		perror("connect");
		irc_free(sock);
		return -1;
	}
	return sock;
}

int main(int argc, char *argv[])
{
	int status;

	char *host = "127.0.0.1";
	char *port = "6667";

	if(argc == 3)
	{
		host = argv[1];
		port = argv[2];
	}

	int sock = irc_connect(host, port);

	if (sock == -1)
	{
		perror("socket");
		return -1;
	}


	//Automatic Registration
	if(send(sock, "NICK nick\r\n", strlen("NICK nick\r\n") + 1, 0) == -1)
	{
		perror("send");
		irc_free(sock);
		return -1;
	}
	if(send(sock, "USER user\r\n", strlen("USER user\r\n") + 1, 0) == -1)
	{
		perror("send");
		irc_free(sock);
		return -1;
	}


	while (1)
	{
		ssize_t r;
		int s;

		char inbuf[MAX_MESSAGE_SIZE + 2];
		memset(&inbuf, 0, sizeof inbuf);
		r = read(STDOUT_FILENO, inbuf, MAX_MESSAGE_SIZE);
		if (r < 1)
		{
			perror("recv");
			irc_free(sock);
			return -1;
		}

		if (inbuf[r - 1] == '\n')
		{
			inbuf[r - 1] = '\r';
			inbuf[r] = '\n';
		}

		r = send(sock, inbuf, r + 1, 0);
		if (r == -1)
		{
			perror("send");
			irc_free(sock);
			return -1;
		}

	}

	irc_free(sock);
	return 0;
}
