#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "Struct.h"

#include <stdint.h>				//Wenn Programma auf Linux

#include <sys/types.h>

#include <sys/socket.h>

#include <arpa/inet.h>


#define BUFFERSIZE 1028

int main(int argc, char * argv[]) {

   char * ipv6adr = argv[1];
   char * sNummer = argv[2];
   short port = atoi(argv[3]);

   int sendlen, txtlen;
   char sendbuf[BUFFERSIZE], sendtext[BUFFERSIZE];
   struct sockaddr_in6 clientaddr;

   int sock = socket(AF_INET6, SOCK_STREAM, 0);

   //Socketfehler
   if (sock < 0) {
      printf("socket ERROR IPV6: ");
      return -1;
   }
   memset( & clientaddr, 0, sizeof(clientaddr));

   clientaddr.sin6_family = AF_INET6;
   clientaddr.sin6_port = htons(port);
   clientaddr.sin6_flowinfo = 0;

   int af;
   inet_pton(AF_INET6, ipv6adr, & (clientaddr.sin6_addr));

   int recvlen;

   if (connect(sock, (struct sockaddr * ) & clientaddr, sizeof(clientaddr)) < 0) {
      printf("CONNECT ERROR IPV6:");
      return -1;
   }

   printf("%s> ", & sNummer);
   for (;;) {

      fd_set readfds;

      FD_ZERO( & readfds);
      FD_SET(sock, & readfds);
      FD_SET(0, & readfds);

      if (select(sock + 1, & readfds, NULL, NULL, NULL) < 0) {
         perror("select error");
      }
      //Keyboard Input and Message send
      if (FD_ISSET(0, & readfds)) {

         fgets(sendbuf, BUFFERSIZE, stdin);
         txtlen = strlen(sendbuf);
         fflush(stdout);
         printf("%s> ", sNummer);
         struct packet message;

         strcpy(message.snummer, sNummer);
         strcpy(message.text, sendbuf);

         if (send(sock, & message, sizeof(struct packet), 0) != sizeof(struct packet)) {
            printf("SEND ERROR IPV6:");
            return -1;
         }
      }
      if (FD_ISSET(sock, & readfds)) {
         struct packet incommingmsg;
         char recvbuf[sizeof(struct packet)];
         char sendbuf[BUFFERSIZE];
         int txtlen;

         recvlen = recv(sock, recvbuf, sizeof(struct packet), 0);
         if (recvlen < 0) {
            printf("RECV ERROR IPV6:");
            return -1;
         }
         memcpy( & incommingmsg, recvbuf, sizeof(recvbuf));
         fflush(stdout);
         printf("%s > %s", incommingmsg.snummer, incommingmsg.text);
      }
   }
   while (1);

   close(sock);
}