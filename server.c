#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Struct.h"
#include <conio.h>


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

#define BUFFERSIZE 1028

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
    char* sNummer = argv[2];


    int clielen, recvlen;

    struct sockaddr_in6 serveraddr, clientaddr;

    SOCKET listeningsocket = socket(AF_INET6, SOCK_STREAM, 0);

    if (listeningsocket < 0) {
        printf("SOCKET ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(7000);


    if (bind(listeningsocket, (struct serveraddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        printf("BIND ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }

    listen(listeningsocket, 5);

    clielen = sizeof(clientaddr);

    SOCKET newsock = accept(listeningsocket, (struct serveraddr*)&clientaddr, &clielen);

    if (newsock < 0) {
        printf("ACCEPT ERROR IPV6: %d\n", WSAGetLastError());
        return -1;
    }

    fd_set readfds;
    printf("%s>", sNummer);
    for (;;) {


       

       
        FD_ZERO(&readfds);

        FD_SET(newsock, &readfds);

        if (select(newsock+1, &readfds, NULL, NULL,NULL) < 0) {
            perror("select error");
        }

    
        if (FD_ISSET(newsock, &readfds)) {


            struct packet incommingmsg;
            char recvbuf[sizeof(struct packet)];
            char sendbuf[BUFFERSIZE];
            int txtlen;

            recvlen = recv(newsock, recvbuf, sizeof(struct packet), 0);
            if (recvlen < 0) {
                printf("RECV ERROR IPV6: %d\n", WSAGetLastError());
                return -1;
            }
            memcpy(&incommingmsg, recvbuf, sizeof(recvbuf));
            printf("%s > %s", incommingmsg.snummer, incommingmsg.text);
        }
        else {
            printf("%s>", sNummer);
            fflush(stdout);

            char buf[BUFFERSIZE];
            fgets(buf, BUFFERSIZE, stdin);

            printf("%s> %s", sNummer, buf);
            struct packet message;
            strcpy(message.snummer, sNummer);
            strcpy(message.text, buf);

            if (send(newsock, &message, sizeof(struct packet), 0) != sizeof(struct packet)) {
                printf("SEND ERROR IPV6: %d\n", WSAGetLastError());
                return -1;
            }

        }

           
            
        
        //RECEIVE MESSAGE
        

    }
        
    


        closesocket(newsock);
        closesocket(listeningsocket);
        return 0;
    }


