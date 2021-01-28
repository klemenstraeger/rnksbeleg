#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Struct.h"
#include <stdint.h>				//Wenn Programma auf Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <fcntl.h>

#define BUFFERSIZE 1028

int main(int argc, char *argv[])
{
	printf("Starte Client ...\n\n");
	char *ipv6adr = argv[1];
	char *sNummer = argv[2];
	short port = atoi(argv[3]);
	
	

	int sendlen, txtlen;
	char sendbuf[BUFFERSIZE], sendtext[BUFFERSIZE];
	struct sockaddr_in6 clientaddr;

	int sock = socket(AF_INET6, SOCK_STREAM , 0);

	//Socketfehler
	if (sock < 0)
	{
		printf("socket ERROR IPV6: ");
		return -1;
	}

	memset(&clientaddr, 0, sizeof(clientaddr));

	clientaddr.sin6_family = AF_INET6;
	clientaddr.sin6_port = htons(port);
	clientaddr.sin6_flowinfo = 0;

	int af;
	inet_pton(AF_INET6, ipv6adr, &(clientaddr.sin6_addr));

	int recvlen;

	if (connect(sock, (struct sockaddr *) &clientaddr, sizeof(clientaddr)) < 0)
	{
		printf("CONNECT ERROR IPV6:");
		return -1;
	}

	struct packet message;

	strcpy(message.snummer, sNummer);
	strcpy(message.text, "-");

	if (send(sock, &message, sizeof(struct packet), 0) != sizeof(struct packet))
	{
		printf("SEND ERROR IPV6:");
		return -1;
	}

	printf("Mit Server verbunden...\n");
	fflush(stdout);
	printf("Beginne einen Chat...\n:");
	fflush(stdout);
	
	int first = 1;
	fd_set readfds;

		
		

	for (;;)
	{
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		FD_SET(0, &readfds);
		
		fflush(stdout);

		if (select(sock + 1, &readfds, NULL, NULL, NULL) < 0)
		{
			perror("select error");
		}

		//Keyboard Input and Message send
		if (FD_ISSET(0, &readfds))
		{	
			
			fgets(sendbuf, BUFFERSIZE, stdin);
			fflush(stdin);
			txtlen = strlen(sendbuf);

			struct packet message;

			strcpy(message.snummer, sNummer);
			strcpy(message.text, sendbuf);
			int sendret = send(sock, &message, sizeof(struct packet), 0);
			if (sendret != sizeof(struct packet))
			{
				printf("SEND ERROR IPV6:");
				return -1;
			}

			if (sendret == -1)
			{
				printf("Server disconnected!");
				break;
			}

			//incomming message
		}

		if (FD_ISSET(sock, &readfds))
		{
			struct packet incommingmsg;
			char recvbuf[sizeof(struct packet)];
			char sendbuf[BUFFERSIZE];
			int txtlen;

			recvlen = recv(sock, recvbuf, sizeof(struct packet), 0);
			if (recvlen < 0)
			{
				printf("RECV ERROR IPV6:");
				return -1;
			}
			 if(recvlen  == 0){
			    printf("Server disconnected!");
			    break;
			 }

			memcpy(&incommingmsg, recvbuf, sizeof(recvbuf));
			fflush(stdout);
			printf("%s> %s", incommingmsg.snummer, incommingmsg.text);
			fflush(stdout);
		}
	}

	close(sock);
}
