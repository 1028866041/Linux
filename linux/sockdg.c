#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
    int sockfd,addrlen,numbytes;
    struct sockaddr_in seraddr;
    char buf[1024];
    char addr_ip[INET_ADDRSTRLEN];
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd< 0)
        return -1;

    memset(&seraddr, 0, sizeof(struct sockaddr_in));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(13);
    seraddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr))<0)
        return -1;
    
    addrlen = sizeof(struct sockaddr); 
    if((numbytes= recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&seraddr, &addrlen))<0)
        return -1;

    inet_ntop(AF_INET, &seraddr.sin_addr, addr_ip, sizeof(addr_ip));
    printf("got packet from %s\r\n", addr_ip);
    printf("packet length %d bytes\t", numbytes);
    buf[numbytes]= '\0';
    printf("packet content %s\n", buf);
    close(sockfd);
    
    return 0;
}

