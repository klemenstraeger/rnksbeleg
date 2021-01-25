#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Struct.h"

					//Wenn Programma auf Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFERSIZE 1028



int main(int argc, char* argv[]) {
    
    printf("\n\nInitlizing Server...\n\n");
    int port = atoi(argv[1]);
    char* sNummer = argv[2];


    int clielen, recvlen;

    struct sockaddr_in6 serveraddr, clientaddr;

    int listeningsocket = socket(AF_INET6, SOCK_STREAM, 0);

    if (listeningsocket < 0) {
        printf("SOCKET ERROR IPV6: ");
        return -1;
    }
    memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(7000);


    if (bind(listeningsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        printf("BIND ERROR IPV6");
        return -1;
    }

    listen(listeningsocket, 5);

    clielen = sizeof(clientaddr);

    int newsock = accept(listeningsocket, (struct sockaddr*)&clientaddr, &clielen);

    if (newsock < 0) {
        printf("ACCEPT ERROR IPV6: ");
        return -1;
    }
    printf("Succesfully connectet to: %s",sNummer);
    
    printf("%s> ",sNummer);
    fd_set readfds;
    
    for (;;) {
       
        FD_ZERO(&readfds);
        FD_SET(newsock, &readfds);
        FD_SET(0,&readfds);

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
                printf("RECV ERROR IPV6:");
                return -1;
            }
            memcpy(&incommingmsg, recvbuf, sizeof(recvbuf));
            fflush(stdout);
            printf("%s > %s", incommingmsg.snummer, incommingmsg.text);
        }
        
        if(FD_ISSET(0,&readfds)){
        
	    fflush(stdout);
	    printf("%s>",sNummer);
            char buf[BUFFERSIZE];
            fgets(buf, BUFFERSIZE, stdin);
	   
            
            struct packet message;
            strcpy(message.snummer, sNummer);
            strcpy(message.text, buf);

            if (send(newsock, &message, sizeof(struct packet), 0) != sizeof(struct packet)) {
                printf("SEND ERROR IPV6:");
                return -1;
            }

        }

           
            
        
        //RECEIVE MESSAGE
        

    }
        
    


        close(newsock);
        close(listeningsocket);
        return 0;
    }


