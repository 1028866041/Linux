#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>

int main(int argc,char **argv){
	int lisfd,confd,n;
	pid_t pid;
	char line[256];
	socklen_t len;
	struct sockaddr_in seraddr,cliaddr; 

	if((lisfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		printf("socket errAF_INETor\r\n");
		return -1;
	}
	memset(&seraddr, 0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(argc ==2)
		seraddr.sin_port = htons(atoi(argv[1]));
	else
		seraddr.sin_port = htons(15);
	
	if(bind(lisfd,(struct sockaddr *)&seraddr,sizeof(seraddr))<0){
		printf("bind error\r\n");
		return -1;
	}	
	if(listen(lisfd,1024)<0){
		printf("listen error\r\n");	
		return -1;
	}
	printf("listening\r\n");
	
	for(;;){
		len = sizeof(cliaddr);
		if((confd = accept(lisfd,(struct sockaddr *)&cliaddr, &len))<0){
			printf("accept error\r\n");
			return -1;
		}

		/*concurrent server */
		if((pid = fork())==0){
			close(lisfd);
			printf("concurrent server do something\r\n");
			if(n==read(confd,line,256)<0)
			{
				printf("readline error\r\rn");
				return -1;
			}
			write(confd,line,n);
			exit(0);
		}
		close(confd);
	}
	return;
}

