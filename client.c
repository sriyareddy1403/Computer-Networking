/* A simple client in the internet domain using TCP
 The port number is passed as an argument */

/* standard include files */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/* This function returns a human-readable error. 
 * It is always a good idea to catch possible errors and return
 * human-readable errors 
 */

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/* The client program */

int main(int argc, char *argv[])
{
    int sockfd, portno, n;			/* note that socket file descriptors are integers */
    struct sockaddr_in serv_addr;	/* this is the data structure that holds the server's info */
    struct hostent *server;

    char buffer[256];				/* buffer to hold the message to send as well as
									 * the received string */
	
	/* need at least two extra arguments: by convention, the first argument is the 
	 * command name itself */
	
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	/* the port number argument is a string, so we need to make it an integer */
    portno = atoi(argv[2]);
	
	/* we get ourselves a socket file descriptor (it's not connected to anything yet) 
	 * Note that we're asking for TCP by stating 'SOCK_STREAM'. UDP would be SOCK_DGRAM
	 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/* if it's negative, that's an error */
    if (sockfd < 0) 
        error("ERROR opening socket");
	
	/* the server's IP address comes from the name server, invoked by 'gethostbyname' */
    server = gethostbyname(argv[1]);
	
	/* a null returned value means something bad happened */
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
	/* always a good idea to zero out memory (in this case, the server info) before use */
    bzero((char *) &serv_addr, sizeof(serv_addr));
	
	/* set up the server info */
    serv_addr.sin_family = AF_INET;  /* we're going to use an IP address */
    bcopy((char *)server->h_addr,	 /* and here it is (we got it from DNS) */
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno); /* this is the port number, we got that from the 
										 * command line */
	
	/* make a connection */
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	
	/* read the message from the command line */
	bzero(buffer,256);
    printf("Please enter the message: ");
    fgets(buffer,255,stdin);
	
	/* write it to the socket */
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
	
	/* clear the buffer again and read a reply */
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
	
	/* write the reply on the screen */
    printf("%s\n",buffer);
	
	/* clean up and done */
    close(sockfd);
    return 0;
}
