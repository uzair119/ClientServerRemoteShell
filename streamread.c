

/*
 Copyright (c) 1986 Regents of the University of California.
 All rights reserved.  The Berkeley software License Agreement
 specifies the terms and conditions for redistribution.

	@(#)streamread.c	6.2 (Berkeley) 5/8/86
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>


#define TRUE 1


typedef struct
{
    char* name;
    char status[10];
	int pid;
    time_t starttime;
    time_t endtime;

}Process;

void handle_sigchld(int sig) 
{ 
    printf("Caught signal %d\n", sig);
}

char* printProcess(Process p);

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it. 
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection. 
 */

int main()
{
	int sock, length;
	struct sockaddr_in server;
	int msgsock;
	char buf[1024];
	int rval;
	int i;
	
	/* Create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Name socket using wildcards */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = 0;
	if (bind(sock, (struct sockaddr *) &server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}
	/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *) &server, (socklen_t*) &length)) {
		perror("getting socket name");
		exit(1);
	}
	printf("Socket has port #%d\n", ntohs(server.sin_port));
	fflush(stdout);

	/* Start accepting connections */
	listen(sock, 5);
	do {
		msgsock = accept(sock, 0, 0);
		if (msgsock == -1)
			perror("accept");
		else 
		{
			struct sockaddr_in addr;
			socklen_t addr_len = sizeof(addr);
			int err = getpeername(msgsock, (struct sockaddr *) &addr, &addr_len);
			if(err == -1)
				perror("Get peername");
			printf("%s\n",inet_ntoa(addr.sin_addr));

			int c = fork();

			


			if(c < 0)
				perror("fork");
			else if (c == 0)
			{
			signal(SIGCHLD, handle_sigchld);
			Process proclist[100]; //list of processes
			int procnum = 0;
			do {
			
			
			bzero(buf, sizeof(buf));
			//write(1,"4.\n",3);
			if ((rval = read(msgsock, buf, 1024)) < 0)
				perror("reading stream message");

			//CHECK FOR CLOSED PROCESSES
			
			int ret = waitpid(-1,NULL,WNOHANG);
			if(ret > 0 )
			{
				for(int y = 0; y < procnum; y++)
				{
					if(proclist[y].pid == ret)
					{
						proclist[y].endtime = time(NULL);
						strcpy(proclist[y].status,"INACTIVE");
						break;
					}
				}
			}




			//write(1,buf,rval);
			i = 0;
			if (rval == 0)
				printf("Ending connection\n");
			else
			{
				buf[rval] = '\0';       //append NULL character at the end of the input string, since read doesn't place it by default
				/*if(strcmp(buff,"0\n") == 0)	//check if a single 0 is entered, signalling termination
					return 1;
				*/		
				//printf("%s",buff);		
				char* token = strtok(buf," ");	//declare string token pointer, use space as delimiter

				if(strcmp(token,"ADD") == 0 || strcmp(token,"add") == 0)
				{
					char result[1024];
					token = strtok(NULL," ");
					double sum = 0;			//storing double value of sum
					while(token != NULL)		//loop ends when token reaches end of string
					{
						sum += atof(token);	//convert string token to integer and add to sum
						token = strtok(NULL," ");
					}
					int length = sprintf(result,"%.2f\n",sum); //store the sum in the result array. Length gives us the 		number of characters written to the char array.
					write(1,result,length);	//output the result on screen
					write(msgsock,result,length);
				}

				else if(strcmp(token,"SUB") == 0 || strcmp(token,"sub") == 0)
				{
					char result[1024];
					token = strtok(NULL," ");
					double sum = 0;
					if(token != NULL)
						sum = atof(token); //storing double value of sum
					token = strtok(NULL," ");			
					while(token != NULL)		//loop ends when token reaches end of string
					{
						sum -= atof(token);	//convert string token to integer and subtract from sum
						token = strtok(NULL," ");
					}
					int length = sprintf(result,"%.2f\n",sum); //store the sum in the result array. Length gives us the 		number of characters written to the char array.
					write(1,result,length);	//output the result on screen
					write(msgsock,result,length);
				}

				else if(strcmp(token,"MUL") == 0 || strcmp(token,"mul") == 0)
				{
					char result[1024];
					token = strtok(NULL," ");
					double sum = 1;			//storing double value of sum
					while(token != NULL)		//loop ends when token reaches end of string
					{
						sum *= atof(token);	//convert string token to integer and multiply to sum
						token = strtok(NULL," ");
					}
					int length = sprintf(result,"%.2f\n",sum); //store the multiple in the result array. Length gives us the 		number of characters written to the char array.
					write(1,result,length);	//output the result on screen
					write(msgsock,result,length);
				}

				else if(strcmp(token,"DIV") == 0 || strcmp(token,"div") == 0)
				{
					char result[1024];
					int flag = 1;
					token = strtok(NULL," ");
					double sum = 0;
					if(token != NULL)
						sum = atof(token); //storing double value of sum
					token = strtok(NULL," ");
					while(token != NULL)		//loop ends when token reaches end of string
					{
						if(atof(token) == 0)
						{
							write(msgsock,"Cannot divide by 0\n\0",20);
							flag = 0;
							break;
						}
						
						sum /= atof(token);	//convert string token to integer and subtract from sum
						token = strtok(NULL," ");
					}
					if(flag)
					{
						int length = sprintf(result,"%.2f\n",sum); //store the sum in the result array. Length gives us the 		number of characters written to the char array.
						write(1,result,length);	//output the result on screen
						write(msgsock,result,length);
					}
				}

				else if(strcmp(token,"HELP\n") == 0 || strcmp(token,"help\n") == 0)
				{
					char* temp = "List of Commands:\nADD <arguments>\nSUB <arguments>\nDIV <arguments>\nMUL <arguments>\nRUN <program name>\nKILL <process name OR pid>\nLIST\nEXIT\n\0";
        			write(msgsock,temp,strlen(temp));
				}
				else if(strcmp(token,"RUN") == 0 || strcmp(token,"run") == 0)
				{
					token = strtok(NULL," ");
					int pipe_status, exec_status, fork_status, pipefd[2];
					if(pipe2(pipefd,O_CLOEXEC) < 0)
						perror("pipe");
					
					if(token[strlen(token)-1] == '\n')
						token[strlen(token)-1] = '\0'; //The last character of the token is \n. I've replaced it with the null character so it can be used in the next instruction.
					
					char program[strlen(token)];
					strcpy(program,token);
					//check for arguments. Allowing upto 3 here
					char* args[5];
					int i = 0;
					token = strtok(NULL," ");
					while(token != NULL && i < 4)
					{
						i++;
						//perror("token");
						args[i] = (char*) malloc(strlen(token));
						//perror("malloc");
						strcpy(args[i],token);
						//perror("cpy");
						token = strtok(NULL," ");
						//printf("arg#%d  %s\n",i, args[i]);
					}

					if(i != 0 && args[i][strlen(args[i])-1] == '\n')
							args[i][strlen(args[i])-1] = '\0';
					args[0] = "";
					args[i+1] = NULL;
					int pid = fork();
					if(pid == 0){	
						if(execvp(program,args) == -1)
						{
							perror("Error");
							write(pipefd[1],"Fail\n",5);
						}
					}
					else if(pid > 0)
					{
						char errbuff[5];
						close(pipefd[1]);
						int check = read(pipefd[0], errbuff, sizeof(errbuff));
						close(pipefd[0]);
						char* succ = "Run successful.\n";
						char* fail = "Run failed.\n";
						if(check == 0)
						{
							write(msgsock,succ,strlen(succ));
							proclist[procnum].name = strcpy(malloc(strlen(program)),program);
							strcpy(proclist[procnum].status,"ACTIVE");
							proclist[procnum].starttime = time(NULL);
							proclist[procnum].pid = pid;
							procnum++;
						}
						else
							write(msgsock,fail,strlen(fail));
					}
					//write(1,"1.\n",2);
				}
				else if(strcmp(token,"LIST\n") == 0 || strcmp(token,"list\n") == 0)
				{
					//write(1,"PLIST\n",6);
					char msg[10000];
					char tmp[10000];
					msg[0] = '\0';
					tmp[0] = '\0';
					strcat(tmp,"PROCESS NAME\tPID\tSTATUS\t\tSTART TIME\t\t\tELAPSED TIME\tEND TIME\t\n");
					strcat(msg,tmp);
					
        			for(int y = 0; y < procnum; y++)
        			{
            			strcpy(tmp,printProcess(proclist[y]));
						strcat(msg,tmp);
        			}
					write(msgsock,msg,strlen(msg));
				}
			
				//write(1,"2.\n",2);
			}
			//write(1,"3.\n",2);
				//printf("-->%s\n", buf);
			} while (rval != 0);
			}
		}
		close(msgsock);
	} while (TRUE);
	/*
	 * Since this program has an infinite loop, the socket "sock" is
	 * never explicitly closed.  However, all sockets will be closed
	 * automatically when a process is killed or terminates normally. 
	 */
}




char* printProcess(Process p)
{
    char* starttime = strcpy(malloc(75),asctime(localtime(&p.starttime)));
    char* endtime;
    char* ptr;
    double elapsed;
    //printf("1");
    if( (ptr = strchr(starttime, '\n')) != NULL)  //remove newline character at the end
            *ptr = '\0';
	char* msg = malloc(1000);
    if(!strcmp(p.status,"ACTIVE"))
    {
        endtime = "---------\n";
        elapsed = difftime(time(NULL),p.starttime);
		
        sprintf(msg,"%s\t\t%d\t%s\t\t%s\t%.0fs\t\t%s",p.name,p.pid,p.status,starttime,elapsed,endtime);
    }
    else
    {
        //printf(starttime);
        endtime =  asctime(localtime(&p.endtime));
        //printf(starttime);
        elapsed = difftime(p.endtime,p.starttime);
        sprintf(msg,"%s\t\t%d\t%s\t%s\t%.0fs\t\t%s",p.name,p.pid,p.status,starttime,elapsed,endtime);
    }
	return msg;


}
