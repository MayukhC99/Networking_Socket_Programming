 /* FTPclient PROCESS */

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

int main (int argc, char *argv[])
{
	int sd,newsd, rc, i=0;
	FILE *fp;
	struct sockaddr_in clientAddr, servAddr,cliaddr;
	char line[MAX_MSG],line1[MAX_MSG],ch;


	if(argc < 3)
	{
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
  	if(sd<0)
	{
    		printf("%s: cannot create stream socket\n", argv[0]);
    		exit(-1);
  	}
  	else
        printf("%s : successfully created stream socket \n", argv[0]);


    rc = bind(sd, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
  	if(rc<0)
	{
        printf("%s: cannot bind port TCP %s\n",argv[0], argv[1]);
        exit(1);
  	}
  	else
        printf("%s: bound local port successfully\n", argv[0]);



    rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  	if(rc<0)
	{
        printf("%s: cannot connect to server\n", argv[0]);
        exit(1);
  	}
 	else
        printf("%s: connected to server successfully\n", argv[0]);



    	printf("ENTER REQUIRED FILE NAME : ");
    	scanf("%s", line);

    	rc = send(sd, line, strlen(line) + 1, 0);
    	if(rc<0)
	{
      		printf("%s: cannot send data\n", argv[0]);
      		close(sd);
      		exit(1);
    	}
    	else
		printf("%s: data sent (%s)\n",argv[0], line);


	recv(sd,line1,MAX_MSG,0);
	printf("\n %s\n",line1);
	if(strcmp(line1,"UNAVAILABLE")==0)
	{
		printf("NO SUCH FILE EXISTS \n");
		close(sd);
		exit(1);
	}
	else
	{
		fp=fopen("pro2.txt","w");
		if(fp==NULL)
		{
		puts("COPYING FAILED<CAN NOT CREATE DESTINATION FILE \n");
		fclose(fp);
		close(sd);
		exit(1);
		}
		else{
			do
			{
				recv(sd,&ch,sizeof(ch),0);
				if(ch!=EOF)
				putc(ch,fp);
			}while(ch!=EOF);
		fclose(fp);
		}
	}

  	printf("%s : closing connection with the server\n", argv[0]);
  	close(sd);
	exit(1);
}





