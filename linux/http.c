#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
int htsend(int sock, char* fmt,...);

/*This program runs with apache tomcat server*/
int main(int argc, char** argv)
{
    int sock;
    char bunny[3];

    if(argc!= 2)
    {
        printf("useage\n");
        return -1;
    }
    
    if((sock= htconnect(/*"192.168.99.100"*/argv[1], 8080))<0)
        return -1;
    htsend(sock, "GET /HTTP/1.0%c", 10);
    htsend(sock, "Host:%s%c", /*"192.168.99.100"*/argv[1], 10);
    htsend(sock, "%c", 10);
    
    while(read(sock,bunny,3)>0)
    {   
        printf("%s", bunny);
    }
    
    printf("\nread from sock done\r\n");
    close(sock);

    return 0;
}

int htconnect(char *name, int port)
{
    int w_sock;
    struct hostent *site;
    struct sockaddr_in httpaddr;

    site = gethostbyname(name);
    if(site==NULL)
        return -1;

    w_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(w_sock< 0)
        return -1;

    memset(&httpaddr, 0, sizeof(struct sockaddr_in));
    memcpy(&httpaddr.sin_addr, site->h_addr_list[0], site->h_length);
    httpaddr.sin_family = AF_INET;
    httpaddr.sin_port = htons(port);
    memcpy(&httpaddr.sin_addr, site->h_addr_list[0], site->h_length);
    /*httpaddr.sin_addr = *(struct in_addr*)site->h_addr_list[0];*/
    
    if(connect(w_sock,(struct sockaddr*)&httpaddr, sizeof(struct sockaddr))<0)
        return -1;

    return w_sock;
}

/*send http header*/
int htsend(int sock, char* fmt,...)
{
    char buf[1024];
    va_list argptr;

    memset(buf, 0 ,1024);
    va_start(argptr, fmt);
    vsprintf(buf, fmt, argptr);
    va_end(argptr);

    return send(sock, buf, strlen(buf), 0);
}

