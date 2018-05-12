#include<stdio.h>
#include<string.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>	

#define LEN 1024
int main(int argc,char **argv){

	int sockfd,n,counter=0;
	char recvline[LEN+1];
	struct sockaddr_in cliaddr;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		printf("socket error\r\n");
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(13);
	#if 0
	if((inet_pton(AF_INET, "192.168.99.100"/*argv[1]*/, &(cliaddr.sin_addr)))<0)
		printf("inet_pton error\r\n");
	#else
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	#endif

	if(connect(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))<0)
		printf("client connect error\r\n");

	while((n=read(sockfd, recvline,LEN))>0){
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
	printf("\nclient counter=%d\r\n",counter);

	return 0;
}

