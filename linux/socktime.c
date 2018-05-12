#include<stdio.h>
#include<string.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>	

int main(){
	
	int sockfd;
	struct sockaddr_in seraddr;
	char buf[4096];
	socklen_t len;
	time_t ticks;
    pid_t pid;
    int num,one=1;
    
    #if 0
    int confd;
    struct sockaddr_in cliaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
  
	memset(&seraddr, 0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(13);
	if(bind(sockfd,(struct sockaddr *)&seraddr, sizeof(seraddr))<0)
		printf("bind error\r\n");
	if(listen(sockfd, 1024)<0)
		printf("listen error\r\n");
	printf("listening\r\n");

	for(;;){
		confd = accept(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
        if(confd< 0)
            return -1;
		printf("\r\nconnection from %s port %d",
				inet_ntop(AF_INET, &(cliaddr.sin_addr), buf,sizeof(buf)),
				ntohs(cliaddr.sin_port));
		ticks = time(NULL);
		snprintf(buf, sizeof(buf),"%.24s\r\n", ctime(&ticks));

		write(confd, buf,strlen(buf));
		close(confd);
	}
    #else
	sockfd = socket(AF_INET,SOCK_DGRAM,0);    
    setsockopt(sockfd, SOL_SOCKET/*setting level*/, SO_REUSEADDR, &one, sizeof(one));
    
	memset(&seraddr, 0,sizeof(seraddr));
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	seraddr.sin_port = htons(13);
	if(bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr))<0)
		return -1;

    len = sizeof(struct sockaddr);
	if((pid=fork())==0)
    {
        /*child process*/        
        num = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&seraddr, &len);
        printf("\nsocket datagram %s with child process %d\n", 
            inet_ntop(AF_INET, &(seraddr.sin_addr), buf,sizeof(buf)), getpid());
        sendto(sockfd, buf, num, 0, (struct sockaddr*)&seraddr, len);
        exit(0);
    }
    exit(0);		
    #endif
	return 0;
}
