/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

/* standard include files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


/* This function returns a human-readable error. 
 * It is always a good idea to catch possible errors and return
 * human-readable errors 
 */

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/* The server program */

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno; /* note that socket file descriptors are integers */
     socklen_t clilen;				/* we need this for the 'accept' call */ 
	
	 char buffer[256];				/* buffer to hold the received string */
	
     struct sockaddr_in serv_addr, cli_addr;
									/* this is the data structure that holds the 
									 * server's own info as well as the client's info */
     int n;
	
	/* need at least one extra arguments: by convention, the first argument is the 
	 * command name itself */
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
	
	/* the port number argument is a string, so we need to make it an integer */
	portno = atoi(argv[1]);
	
	/* we get ourselves a socket file descriptor (it's not connected to anything yet) 
	 * Note that we're asking for TCP by stating 'SOCK_STREAM'. UDP would be SOCK_DGRAM
	 */
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
	
	/* always a good idea to zero out memory (in this case, the server info) before use */
     bzero((char *) &serv_addr, sizeof(serv_addr));
	
	/* set up the server info */
     serv_addr.sin_family = AF_INET;			/* Internet socket */
     serv_addr.sin_addr.s_addr = INADDR_ANY;	/* accept connections from anyone */
     serv_addr.sin_port = htons(portno);		/* port to listen on*/
	
	/* bind the server info to the socket */
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
    
	/* listen on the socket, queueing up to 5 requests */
	 listen(sockfd,5);
	
	/* if we got here, some client connected */
     clilen = sizeof(cli_addr);
	
	/* accept the connection and get a new socket descriptor */
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
	
	/* clean out buffer and read the message */
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
	
	/* write the message on the screen */
     printf("Here is the message: %s\n",buffer);
	
	/* acknowledge the message on the connection */
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
	
	/* clean up and exit */
     close(newsockfd);
     close(sockfd);
     return 0; 
}
