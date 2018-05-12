#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>	

int main(int argc, char **argv)
{
	int	listenfd, connfd, num;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;	
	pid_t pid;
	char buff[1025] = {'\0'};

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);

	if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0){
		printf("bind error\r\n");
		return -1;
	}
	if(listen(listenfd, 1024)<0){
		printf("listen error\r\n");
		return -1;
	}
	printf("listening\r\n");

	/*single server*/
	for(;;){
		len = sizeof(cliaddr);
		if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len))<0){
			printf("accept failed\r\n");
			return -1;
		}
		
		printf("connection from %s, port %d\n",
		   inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
		   ntohs(cliaddr.sin_port));
		
		#if 0
		/*cation boundary out of range*/
	    write(connfd, buff, strlen(buff));
		if((num = recvfrom(connfd, buff, 1024, 0, (struct sockaddr *)&cliaddr, sizeof(struct sockaddr)))<0){
			printf("recvfrom error\r\n");	
			return -1;
		}
		#else
		/*server */
		if((pid = fork())==0){
			/*parent thread */
			close(listenfd);     
            write(connfd, buff, num);
			while(num==read(connfd,buff, 256)<0)
			{
				printf("readline error\r\rn");
				return -1;
			}			
            printf("\r\nserver do something");
		}
		close(connfd);
		#endif
		//printf("client got packet from %s num %d\r\n", inet_ntoa(cliaddr.sin_addr), num);
	
		close(listenfd);
		exit(0);
	}
	
	return;
}

