/* tcpClient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MAX_MSG 100

int main (int argc, char *argv[]) {

  int sd, rc, i;
  struct sockaddr_in servAddr, clientAddr;
  char line[MAX_MSG];

  /***************************/
  /* check command line args */
  /***************************/

  if(argc < 3) {
    printf("usage: %s <server-addr> <server-port>\n",argv[0]);
    exit(1);
  }

  /**********************************/
  /* build server address structure */
  /**********************************/

  bzero(&servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  servAddr.sin_port = htons(atoi(argv[2]));

  /**********************************/
  /* build client address structure */
  /**********************************/

  bzero(&clientAddr, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = INADDR_ANY;
  clientAddr.sin_port = htons(0);

  /************************/
  /* create stream socket */
  /************************/

  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    printf("%s: cannot create stream socket\n", argv[0]);
    exit(-1);
  }
  else
    printf("%s : successfully created stream socket \n", argv[0]);

  /**************************/
  /* bind local port number */
  /**************************/

  rc = bind(sd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
  if(rc<0) {
    printf("%s: cannot bind port TCP %s\n",argv[0], argv[1]);
    exit(1);
  }
  else
    printf("%s: bound local port successfully\n", argv[0]);

  /*********************/
  /* connect to server */
  /*********************/

  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    printf("%s: cannot connect to server\n", argv[0]);
    exit(1);
  }
  else
    printf("%s: connected to server successfully\n", argv[0]);

    /***********************/
    /* send data to server */
    /***********************/

  do{
    printf("Enter string to send to server : ");
    fgets(line, MAX_MSG, stdin);
    //scanf("%s", line);

    line[strcspn(line, "\n")] = 0;

    rc = send(sd, line, strlen(line) + 1, 0);
    if(rc<0) {
      printf("%s: cannot send data\n", argv[0]);
      close(sd);
      exit(1);
    }
    printf("%s: data sent (%s)\n",argv[0], line);
  }while(strcmp(line, "quit"));


  printf("%s : closing connection with the server\n", argv[0]);
  close(sd);
}

