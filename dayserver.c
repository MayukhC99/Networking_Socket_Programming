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

	int main (int argc, char *argv[])
	{

  		int sd,sd1, newSd, cliLen=0, n,i=0;
  		float jj=0;
		FILE *fp;
  		struct sockaddr_in cliAddr, servAddr,clientAddr;
  		char line[MAX_MSG],ch;

  		if(argc < 3) {
            printf("usage: %s <server-addr> <server-port>\n",argv[0]);
            exit(1);
        }

  		bzero((char *)&servAddr, sizeof(servAddr));
  		servAddr.sin_family = AF_INET;
  		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
  		servAddr.sin_port = htons(atoi(argv[2]));

  		sd = socket(AF_INET, SOCK_STREAM, 0);

  		if(sd<0)
		{
            printf("%s : cannot create stream socket \n", argv[0]);
            exit(-1);
  		}
  		else
            printf("%s : successfully created stream socket \n", argv[0]);

        if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0)
		{
            printf("%s : cannot bind port \n", argv[0]);
    		exit(-1);
  		}
  		else
            printf("%s : bound local port successfully\n", argv[0]);


  		listen(sd,5);


        printf("%s: waiting for client connection on port TCP %u\n",argv[0],atoi(argv[2]));

        cliLen=sizeof(cliAddr);

  		newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
  		if(newSd<0)
		{
    			printf("%s : cannot accept connection \n", argv[0]);
    			exit(-1);
  		}
  		else
            printf("%s: received connection from host [IP %s ,TCP port %d]\n",argv[0],inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

  	        memset(line,0x0,MAX_MSG);
    		n=recv(newSd, line, MAX_MSG, 0);

        printf("%s: received from host [IP %s ,TCP port %d] : %s\n", argv[0],inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port),line);

		if(strcmp(line,"yes")==0)
 		{
    		system("date>day.txt");
            fp=fopen("day.txt","r+");
            if(fp==NULL)
            {
                printf("ERROR opening file or no such file");
                strcpy(line,"UNAVAILABLE");
                send(newSd,line,strlen(line)+1,0);
            }
            else
            {
                strcpy(line,"AVAILABLE");
                send(newSd,line,strlen(line)+1,0);
                for(jj=0;jj<=100;)
                    jj=jj+.00001;
                printf("\n SENDING DATA....\n");
                printf("\n------------------------------\n");
                do
                {
                    ch=fgetc(fp);
                    send(newSd,&ch,sizeof(ch),0);
                    if(ch!=EOF)
                        printf("%c",ch);

                }while(ch!=EOF);
                fclose(fp);
                printf("\n -----------------------------\n");
                printf("DATA SENT::\n");

            }
		}
		else
		{
 		strcpy(line,"UNAVAILABLE");
                        send(newSd,line,strlen(line)+1,0);

		printf("%s: closing connection with host [IP %s ,TCP port %d]\n",argv[0],inet_ntoa(cliAddr.sin_addr),
                    ntohs(cliAddr.sin_port));
		close(newSd);
		exit(1);
		}
        printf("%s: closing connection with host [IP %s ,TCP port %d]\n",argv[0],inet_ntoa(cliAddr.sin_addr),
        ntohs(cliAddr.sin_port));

            close(newSd);
            exit(1);

	}



