#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Struct.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#define BUFFERSIZE 1028

int main(int argc, char *argv[])
{
	int port = atoi(argv[1]);
	char *sNummer = argv[2];
	printf("\n\nInitlizing Server...\n\n");
	printf("Name: %s\nPort: %d\n", sNummer, port);

	int clielen, recvlen;

	struct sockaddr_in6 serveraddr, clientaddr;
	int listeningsocket = socket(AF_INET6, SOCK_STREAM, 0);

	if (listeningsocket < 0)
	{
		perror("Socket error");
		return -1;
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(7000);

	if (bind(listeningsocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("Bind error");
		return -1;
	}

	listen(listeningsocket, 30);
	clielen = sizeof(clientaddr);

	//Nonblocking socket
	int incsock = accept4(listeningsocket, (struct sockaddr *) &clientaddr, &clielen, SOCK_NONBLOCK);

	if (incsock < 0)
	{
		perror("Accept error");
		return -1;
	}

	struct packet incommingmsg;
	char recvbuf[sizeof(struct packet)];

	recvlen = recv(incsock, recvbuf, sizeof(struct packet), 0);
	if (recvlen < 0)
	{
		perror("Recv Error error");
		return -1;
	}

	memcpy(&incommingmsg, recvbuf, sizeof(recvbuf));
	printf("Verbunden mit: %s\n", incommingmsg.snummer);
	fflush(stdout);

	printf("Beginne einen Chat...\n:");
	fflush(stdout);

	fd_set readfds;

	for (;;)
	{
		FD_ZERO(&readfds);
		FD_SET(incsock, &readfds);
		FD_SET(0, &readfds);

		fflush(stdout);

		if (select(incsock + 1, &readfds, NULL, NULL, NULL) < 0)
		{
			perror("select error");
		}

		//Eigehende Nachicht von Client
		if (FD_ISSET(incsock, &readfds))
		{
			struct packet incommingmsg;
			char recvbuf[sizeof(struct packet)];

			recvlen = recv(incsock, recvbuf, sizeof(struct packet), 0);
			if (recvlen < 0)
			{
				printf("RECV ERROR IPV6:");
				return -1;
			}

			if (recvlen == 0)
			{
				printf("Client disconnected!");
				break;
			}

			memcpy(&incommingmsg, recvbuf, sizeof(recvbuf));
			printf("%s > %s", incommingmsg.snummer, incommingmsg.text);
			fflush(stdout);

		}

		if (FD_ISSET(0, &readfds))
		{
			char buf[BUFFERSIZE];
			fgets(buf, BUFFERSIZE, stdin);

			struct packet message;
			strcpy(message.snummer, sNummer);
			strcpy(message.text, buf);

			int sendret = send(incsock, &message, sizeof(struct packet), 0);

			if (sendret != sizeof(struct packet))
			{
				printf("SEND ERROR:");
				return -1;
			}

			if (sendret == -1)
			{
				printf("Client disconected!")
				break;
			}
		}
	}

	close(incsock);
	close(listeningsocket);
	return 1;
}
