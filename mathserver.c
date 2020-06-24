/* SERVER process */

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

  int sd,sd1, newSd, cliLen, n,i=0,op[3],res=0;
  struct sockaddr_in cliAddr, servAddr,clientAddr;
  char line[MAX_MSG],opr[MAX_MSG];

  if(argc < 3) {
    printf("usage: %s <server-addr> <server-port>\n",argv[0]);
    exit(1);
  }

  bzero((char *)&servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  servAddr.sin_port = htons(atoi(argv[2]));

  sd = socket(AF_INET, SOCK_STREAM, 0);

  if(sd<0) {
    printf("%s : cannot create stream socket \n", argv[0]);
    exit(-1);
  }
  else
    printf("%s : successfully created stream socket \n", argv[0]);

  if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0) {
    printf("%s : cannot bind port \n", argv[0]);
    exit(-1);
  }
  else
    printf("%s : bound local port successfully\n", argv[0]);


  listen(sd,5);


  printf("%s: waiting for client connection on port TCP %u\n",argv[0],atoi(argv[2]));

  cliLen=sizeof(cliAddr);
  newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
  if(newSd<0) {
    printf("%s : cannot accept connection \n", argv[0]);
    exit(-1);
  }
  else
    printf("%s: received connection from	host[IP%s,TCPport%d]\n",argv[0],inet_ntoa(cliAddr.sin_addr),
      ntohs(cliAddr.sin_port));

 do{
    memset(line,0x0,MAX_MSG);
    n=recv(newSd, line, MAX_MSG, 0);
    op[i]=atoi(line);

    printf("%s: received from host [IP %s ,TCP port %d] : %s\n",
           argv[0],inet_ntoa(cliAddr.sin_addr),
           ntohs(cliAddr.sin_port), line);
    i++;
  }while(i<2);

  memset(opr,0x0,MAX_MSG);
  recv(newSd,opr,MAX_MSG,0);
  printf("%s: received from host [IP %s ,TCP port %d] : %s\n", argv[0],
           inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), opr);

  if(*opr=='+')
  {
    res=op[0]+op[1];
    printf("The Result obtained : %d\n",res);
  }
  else if(*opr=='-')
  {
    res=op[0]-op[1];
    printf("The Result obtained : %d\n",res);
  }
  else if(*opr=='*')
  {
    res=op[0]*op[1];
    printf("The Result obtained : %d\n",res);
  }
  else if(*opr=='/')
  {
    res=op[0]/op[1];
    printf("The Result obtained : %d\n",res);
  }
  else if(*opr=='%')
  {
    res=op[0]%op[1];
    printf("The Result obtained : %d\n",res);
  }
  else
  {
    res=-9990001;
    printf("Unrecognized Symbol ( %s )\n",opr);
  }

  send(newSd,&res,sizeof(res),0);
  printf("%s: Result Sent ( %d )\n",argv[0],res);

  printf("%s: closing connection with host [IP %s ,TCP port %d]\n",argv[0],inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

  close(newSd);

}



