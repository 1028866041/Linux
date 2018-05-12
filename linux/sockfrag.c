#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/ioctl.h>

#define MAXLINE 5
/*this program run with socktime.c*/
int main(int argc, char **argv)
{
    int sockfd, n, nq;
    struct sockaddr_in cliaddr;
    struct hostent *he;
    int numbytes;
    char buf[1024] = {'\0'};
    char addr_ip[INET_ADDRSTRLEN];
	char recvline[MAXLINE+1],str=NULL;
	socklen_t len;
    
    if(argc!=2)
    {
        fprintf(stderr, "useage\r\n");
        return -1;
    }
    
    if((he= gethostbyname(/*"192.168.99.100"*/argv[1]))==NULL)
        return -1;

    if((sockfd= socket(AF_INET, SOCK_DGRAM, 0))<0)
        return -1;

    /*Udp protocol*/
    memset(&cliaddr, 0, sizeof(struct sockaddr_in));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(13);
    cliaddr.sin_addr = *(struct in_addr *)he->h_addr_list[0];
    bzero(&(cliaddr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))<0)
        return -1;

    len = sizeof(cliaddr);
	getpeername(sockfd, (struct sockaddr *)&cliaddr, &len);
    if(inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)))
        return -1;
	printf("connected to %s\n", str);

	write(sockfd, " ", 2);
    memset(recvline, 0, sizeof(recvline));
	n = recv(sockfd, recvline, MAXLINE, MSG_PEEK);
	ioctl(sockfd, FIONREAD, &nq); /*FIONREAD: return buffer bytes*/
	printf("%d bytes from peek  %d bytes pending MAXLINE=%d\n", n, nq, MAXLINE);

	n = read(sockfd, recvline, MAXLINE);
	recvline[n] = 0;
	fputs(recvline, stdout);
    close(sockfd);    

    return 0;
}

