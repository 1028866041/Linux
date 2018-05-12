#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>

int main(int argc,char **argv){
	int lisfd,confd,n,counter=0;
	pid_t pid;
	char line[256];
	char recvline[1025];
	struct sockaddr_in cliaddr; 
	
	if((lisfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		printf("socket error\r\n");
		return -1;
	}
	memset(&cliaddr, 0,sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	if(argc !=2)
		cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		if(inet_pton(AF_INET, argv[1], &(cliaddr.sin_addr))<0)
		{
			printf("inet_pton error\r\n");
			return -1;
		}
	}

	cliaddr.sin_port = htons(15);
	printf("connecting\r\n");	

	/*client request */
	if((pid=fork())==0){
		if((confd= connect(lisfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)))<0){
			printf("client connect error\r\n");
		  	return -1;
		}
		while((n=read(lisfd, recvline,1025))>0){
			counter++;
			recvline[n]= '\0';
			if(fputs(recvline,stdout) == -1){
				printf("fputs stdout error\r\n");
				break;
			}
		}
		if(n<0){
			printf("read error\r\n");
			return;
		}
		printf("\r\nclient from child counter=%d\r\n",counter);
		
		close(confd);
	}
	close(lisfd);

	return;
}


