/*
 Copyright (c) 1986 Regents of the University of California.
 All rights reserved.  The Berkeley software License Agreement
 specifies the terms and conditions for redistribution.

	@(#)streamwrite.c	6.2 (Berkeley) 5/8/86
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define DATA "Half a league, half a league . . ."

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber 
 */

int main(int argc, char *argv[])
	{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;
	char socketbuf[10000];
	char keyboardbuf[10000];
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Connect socket using name specified by command line. */
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(2);
	}
	bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
	server.sin_port = htons(atoi(argv[2]));

	if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0) {
		perror("connecting stream socket");
		exit(1);
	}
	int rv;
	struct pollfd fds[2];

	fds[0].fd = 0;
	fds[0].events = POLLIN;

	fds[1].fd = sock;
	fds[1].events = POLLIN;

	while(1){
		

		int rv = poll(fds,2,-1); //INFINITE TIMEOUT
		if(rv == -1)
			perror("poll");

		if(fds[0].revents & POLLIN)
		{
			
			int c = read(0,keyboardbuf,sizeof(keyboardbuf));
			if(c < 0)
				perror("Read from keyboard");
			else
				write(sock,keyboardbuf,c);
			
		}
		if(fds[1].revents & POLLIN)
		{
			
			int c = read(sock,socketbuf,sizeof(socketbuf));
			if(c < 0)
				perror("Read from socket");
			else if (c == 0)
			{
				char* msg = "Lost connection to server...\n";
				write(1,msg,strlen(msg));
				break;
			}
			else
				write(1,socketbuf,c);
			
			//fflush(stdout);
		}
		
		//getchar();
	}
	  
	
	close(sock);
}
