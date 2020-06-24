/* CLIENT process */

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

  int sd,newSd, rc, i,res=5,cliLen;
  struct sockaddr_in clientAddr, servAddr,cliAddr;
  char line[MAX_MSG];

  if(argc < 3) {
    printf("usage: %s <server-addr> <server-port>\n",argv[0]);
    exit(1);
  }

  bzero((char *)&servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  servAddr.sin_port = htons(atoi(argv[2]));

  bzero((char *)&clientAddr, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = INADDR_ANY;
  clientAddr.sin_port = htons(0);

  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
    printf("%s: cannot create stream socket\n", argv[0]);
    exit(-1);
  }
  else
    printf("%s : successfully created stream socket \n", argv[0]);

  rc = bind(sd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
  if(rc<0) {
    printf("%s: cannot bind port TCP %s\n",argv[0], argv[1]);
    exit(1);
  }
  else
    printf("%s: bound local port successfully\n", argv[0]);

  rc = connect(sd,(struct sockaddr *)&servAddr,sizeof(servAddr));
  if(rc<0) {
    printf("%s: cannot connect to server\n", argv[0]);
    exit(1);
  }
  else
    printf("%s: connected to server successfully\n", argv[0]);


    printf("Enter first operand to send to server : ");
    scanf("%s", line);
    rc = send(sd, line, strlen(line) + 1, 0);
    printf("%s: data sent (%s)\n",argv[0], line);

    printf("Enter second operand to send to server : ");
    scanf("%s", line);
    rc = send(sd, line, strlen(line) + 1, 0);
    printf("%s: data sent (%s)\n",argv[0], line);

    printf("Enter operator to send to server : ");
    scanf("%s", line);
    rc = send(sd, line, strlen(line) + 1, 0);
    printf("%s: data sent (%s)\n",argv[0], line);

  recv(sd,&res,sizeof(res),0);
  if(res!=-9990001)
    printf("Received Result from server : %d\n",res);
  else{
    printf("Error Message from Server : Unrecognized Symbol ( %s ), ",line);
    printf("try \'+\', \'-\', \'*\', \'/\' or \'%%\' \n");
  }
  printf("%s : closing connection with the server\n", argv[0]);
  close(sd);

}















