#include<stdio.h>
#include<string.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>	
#include<netdb.h>

int main(int argc, char** argv[])
{
    int sendfd, recvfd, n;
    const int on = 1;
    socklen_t salen;
    struct addrinfo hints, res;
	struct sockaddr *sasend, *sarecv;
    
    if(argc!= 3)
    {
        printf("useage\n");
        return -1;
    }
    
    bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
    if((n = getaddrinfo(argv[1]/*host*/, argv[2]/*serv*/, &hints, &res)) != 0)
    {
		printf("udp_client error for %s, %s:\n%s\n", argv[1], argv[2], gai_strerror(n));   
        return -1;
    } 

    memset(&res, 0, sizeof(struct addrinfo));
    /*res.ai_family= AF_UNSPEC;
      res.ai_socktype= SOCK_DGRAM;
      res.ai_protocol= 0;*/
    salen= sizeof(res.ai_addrlen);
    if(sendfd= socket(res.ai_family, res.ai_socktype, res.ai_protocol)<0)
        return -1;
    /*res->ai_addr/res->ai_addrlen*/
    sarecv= malloc(salen);
    memcpy(sarecv, res.ai_addr, salen);
    
    if((recvfd= socket(AF_INET, SOCK_DGRAM, 0))<0)
        return -1;
    if(setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)));
        return -1;
        
    sasend= malloc(salen);
    memcpy(sasend, sarecv, sizeof(salen));
    if(bind(recvfd, sasend, salen)<0)
        return -1;
        
    mcast_join(recvfd, sasend, salen, NULL, 0);
    mcast_set_loop(sendfd, 0);

    if(fork()==0)
    {
        /*child process*/        
        recv_all(recvfd, salen);
    }
    send_all(sendfd, sasend, salen);

    return 0;
}

