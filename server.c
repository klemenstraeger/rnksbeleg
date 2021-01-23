#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifdef _WIN32			//Wenn Programm auf Windowsrechner gestartet wird werden folgende Bibiliotheken geladen
#include <WinSock2.h>
#include <ws2tcpip.h>
#else					//Wenn Programma auf Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#define BUFFERSIZE 124

int setupWinSock() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }
}
void main(int argc, char* argv[]) {

    if (setupWinSock()) {
        abort();
    }
    int port = atoi(argv[1]);

    
    
    int clielen, recvlen;
    char recvbuf[BUFFERSIZE];
    struct sockaddr_in6 serveraddr, clientaddr;

    SOCKET sock = socket(AF_INET6, SOCK_STREAM, 0);

    if (sock < 0) {
        printf("SOCKET ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(7000);
    

    if (bind(sock, (struct serveraddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        printf("BIND ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }

    listen(sock, 5);

    clielen = sizeof(clientaddr);

    SOCKET newsock = accept(sock, (struct serveraddr*)&clientaddr, &clielen);

    if (newsock < 0) {
        printf("ACCEPT ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }
    recvlen = recv(newsock, recvbuf, BUFFERSIZE, 0);

    if (recvlen < 0) {
        printf("RECV ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }
    recvbuf[recvlen] = 0;
    printf("%s\n", recvbuf);


    close(newsock);
    close(sock);
    return 0;
}