#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#ifdef _WIN32	
#include <winsock2.h>//Wenn Programm auf Windowsrechner gestartet wird werden folgende Bibiliotheken geladen
#include <ws2tcpip.h>
#include <io.h>
#else					//Wenn Programma auf Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif



#define BUFFERSIZE 124
#define _BSD_SOURCE

int setupWinSock() {
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return 1;
	}
}

int main(int argc, char* argv[]) {

	if (setupWinSock()) {
		abort();
	}


	char* ipv6adr = argv[1];
	char* sNummer = argv[3];
	short port = atoi(argv[2]);

	int sendlen;
	char sendbuf[BUFFERSIZE];
	struct sockaddr_in6 clientaddr;

	SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);

	//Socketfehler
	if (sock < 0) {
		printf("socket ERROR IPV6: %d\n", WSAGetLastError());
		return -1;
	}
	memset(&clientaddr, 0, sizeof(clientaddr));

	clientaddr.sin6_family = AF_INET6;
	clientaddr.sin6_port = htons(7000);
	clientaddr.sin6_flowinfo = 0;

	int af;
	inet_pton(AF_INET6, ipv6adr, &(clientaddr.sin6_addr));


	strcpy(sendbuf, "Moin");
	sendlen = strlen(sendbuf);

	if (connect(sock, (struct sockaddr*)&clientaddr, sizeof(clientaddr)) < 0) {
		printf("CONNECT ERROR IPV6: %d\n", WSAGetLastError());
		return -1;
	}

	if (send(sock, sendbuf, strlen(sendbuf), 0) != sendlen) {
		printf("SEND ERROR IPV6: %d\n", WSAGetLastError());
		return -1;
	}

	_close(sock);
}

