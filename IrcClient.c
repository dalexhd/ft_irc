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

/* QUES(jeremy): does his have the right scope? */
struct addrinfo *servinfo;

void irc_free(int sockfd)
{
	freeaddrinfo(servinfo);
	if (close(sockfd) == -1)
	{
		perror("connect close");
	}
}

/* QUES(jeremy): return an irc struct?
 * TODO(jeremy): rename to dial?
 */
int irc_connect(char *host, char *port)
{
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(host, port, &hints, &servinfo);
	/* TODO(jeremy): don't assume the first address in servinfo is good */

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

	/* QUES(jeremy): can i free servinfo here? do i want to? */

	return sock;
}

int irc_send_command(int sockfd, char *buf, size_t len)
{
	ssize_t r;
	r = send(sockfd, buf, len, 0);
	return r;
}

/* TODO: synchronize writes to stdout */

int main(int argc, char *argv[])
{
	int status;

	char *host = "169.254.104.52";
	char *port = "6777";

	int sock = irc_connect(host, port);

	if (sock == -1)
	{
		perror("socket");
		return -1;
	}

	int nfds = STDOUT_FILENO;
	if (sock > nfds)
	{
		nfds = sock;
	}
	++nfds;

	int MAX_MESSAGE_SIZE = 512;

	int s;

	while (1)
	{
		fd_set rfds;
		fd_set wfds;

		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(sock, &rfds);
		FD_SET(STDOUT_FILENO, &wfds);
		FD_SET(sock, &wfds);

		s = select(nfds, &rfds, &wfds, NULL, NULL);

		if (s == -1)
		{
			perror("select");
			irc_free(sock);
			return -1;
		}

		if (FD_ISSET(STDIN_FILENO, &rfds))
		{
			/* read input */
			ssize_t r;

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

			/* ensure we can write to sock */
			if (!FD_ISSET(sock, &wfds))
			{
				/* select on sock for writing */
				fd_set wfds_sock;
				FD_SET(sock, &wfds_sock);
				s = select(sock + 1, NULL, &wfds_sock, NULL, NULL);
				/* always going to be sock or error */
				if (s == -1)
				{
					perror("select");
					irc_free(sock);
					return -1;
				}
			}

			/* write to socket */
			r = send(sock, inbuf, r + 1, 0);
			if (r == -1)
			{
				perror("send");
				irc_free(sock);
				return -1;
			}
		}

		if (FD_ISSET(sock, &rfds))
		{
			/* read socket */
			ssize_t r;

			char buf[MAX_MESSAGE_SIZE + 1];
			memset(&buf, 0, sizeof buf);
			r = recv(sock, (void *) &buf, MAX_MESSAGE_SIZE, 0);
			if (r < 1)
			{
				perror("recv buf");
				irc_free(sock);
				return -1;
			}

			if (buf[MAX_MESSAGE_SIZE] != '\0')
			{
				char ovr[MAX_MESSAGE_SIZE];
				/* truncate message to inclde CRLN
				buf[MAX_MESSAGE_SIZE - 2] = '\r';
				buf[MAX_MESSAGE_SIZE - 1] = '\n';*/
				/* drop bytes over the initial 512 */
				while (r == MAX_MESSAGE_SIZE)
				{
					r = recv(sock, (void *) &ovr, MAX_MESSAGE_SIZE, 0);
					if (r < 1)
					{
						perror("recv ovr");
						irc_free(sock);
						return -1;
					}
				}
			}

			/* ensure we can write to stdout */
			if (FD_ISSET(STDOUT_FILENO, &wfds))
			{
				/* select on stdout for writing */
				fd_set wfds_stdout;
				FD_SET(STDOUT_FILENO, &wfds_stdout);
				s = select(STDOUT_FILENO + 1, NULL, &wfds_stdout, NULL, NULL);
				/* always going to be stdout or error */
				if (s == -1)
				{
					perror("select");
					return -1;
				}
			}

			/* write to stdout */
			printf("%s", buf);
		}
	}

	irc_free(sock);
	return 0;
}
