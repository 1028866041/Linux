#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/wait.h>

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in cliaddr;
    struct hostent *he;
    int numbytes;
    char buf[1024] = {'\0'};
    char addr_ip[INET_ADDRSTRLEN];

    if(argc!=3)
    {
        fprintf(stderr, "useage\r\n");
        return -1;
    }
    
    if((he=gethostbyname(/*"192.168.99.100"*/argv[1]))==NULL)
        return -1;

    if((sockfd= socket(AF_INET, SOCK_DGRAM, 0))<0)
        return -1;

    memset(&cliaddr, 0, sizeof(struct sockaddr_in));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(13);
    cliaddr.sin_addr = *(struct in_addr *)he->h_addr_list[0];
    bzero(&(cliaddr.sin_zero), 8);
    
    /*Udp protocol*/
    sprintf(buf, "%s\r\n", argv[2]);
    if((numbytes= sendto(sockfd, buf, strlen(buf), 0,
            (struct sockaddr*)&cliaddr, sizeof(struct sockaddr)))< 0)
        return -1;

    inet_ntop(AF_INET, &cliaddr.sin_addr, addr_ip, sizeof(addr_ip));
    printf("sent %d bytes to %s\r\n", numbytes, addr_ip);
    close(sockfd);
    
    return 0;
}

