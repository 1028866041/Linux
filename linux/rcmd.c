#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<netdb.h>
#include<arpa/inet.h>

/*this program runs with server.c & client.c*/
int main(int argc, int **argv[])
{
	char *ahost = NULL;
	int sockfd,sockfd2,socktmp;
	int lport,rport,num[1024],len;
	struct sockaddr_in seraddr,svaddr;
	struct hostent *hp;
	fd_set readfds;
	char hname[] = "192.168.99.100";
	char cliuname[1024],seruname[1024],cmd[1024],c;

	if(argc!=2)
		ahost = hname;
	else
		ahost = argv[1];

	rport = htons(13);
	hp = gethostbyname(ahost);
	ahost = hp->h_name;	
	for(;;)
	{
		memset(&seraddr, 0, sizeof(seraddr));
		#if 1
		sockfd = rresvport(&lport);			
		#else
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		seraddr.sin_family = AF_INET;
		seraddr.sin_port = htons(13);
		if((inet_pton(AF_INET, "192.168.99.100"/*argv[1]*/, &(seraddr.sin_addr)))<0)
			printf("inet_pton error\r\n");	
		#endif
		fcntl(sockfd, F_SETOWN, getpid());		
		seraddr.sin_family = hp->h_addrtype;
		memcpy(&seraddr.sin_addr, hp->h_addr_list[0], hp->h_length);
		seraddr.sin_port = rport;

		if(connect(sockfd, (struct sockaddr *)&seraddr,sizeof(seraddr)) >=0)
			break;
		close(sockfd);

		if(hp->h_addr_list[1] !=NULL){
			hp->h_addr_list++;
			memcpy(&hp->h_addr_list[0], (caddr_t)&seraddr.sin_addr, hp->h_length);
			fprintf(stderr, "trying %s...\n");
			inet_ntoa(seraddr.sin_addr);	
			continue;
		}	
	  }
	
	  #if 0
	  lport = htons(15);
	  socktmp = rresvport(&lport);		
	  #else		
	  socktmp = socket(AF_INET, SOCK_STREAM, 0);
	  memset(&svaddr, 0, sizeof(svaddr));
	  svaddr.sin_family = AF_INET;
	  svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	  svaddr.sin_port = htons(15);
	  if(bind(socktmp , (struct sockaddr *)&svaddr, sizeof(svaddr))<0)
	  {
	 	printf("bind error\r\n");
 		return -1;
	  }
	  #endif
	  listen(socktmp, 256/*connect queue num*/);

	  sprintf(&num, "%d", lport);
	  printf("write sockfd num=%p\r\n", num);
	  write(sockfd, num, strlen(num)+1);
	  memset(&readfds, 0, sizeof(fd_set));
   	  FD_SET(sockfd, &readfds);
	  FD_SET(socktmp, &readfds);

	  /*select: IO multiplexing*/
	  if(select(32, &readfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0)<0)
	    exit(1);
	  /*socktmp is ready so using it to accept request*/
	  FD_ISSET(socktmp, &readfds);
	  len = sizeof(svaddr);
			
	  if((sockfd2 = accept(socktmp,(struct sockaddr *)&svaddr ,&len))<0)
      {
	 	printf("accept error\r\n");
		return -1;
	  }

      svaddr.sin_port = ntohs(svaddr.sin_port);	
	  close(socktmp);
	  close(sockfd2);

	  write(sockfd, cliuname, strlen(cliuname)+1);
	  //write(sockfd, cmd, strlen(cmd)+1);
	  read(sockfd, &c,1);
	  if(c != 0){
	 	while(read(sockfd, &c,1)){
			write(2,&c,1);
			if(c=='\n'){
				printf("%c", c);
				break;
			}
		}
	  }
	  printf("exec rcmd success\r\n");
	  
	return 0;
}
