#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>
#include<sys/ioctl.h>
#include<netinet/in.h>	
#include<netdb.h>
#include<arpa/inet.h>
#include<pwd.h>
#include<grp.h>

/*this program runs with server.c*/
int main(int argc, int **argv[])
{
	struct sockaddr_in cliaddr,cli_addr,*cli_addrp;
	struct linger linger;
    int sockfd,sockfd2;
    int clisecport,oursecport;
    int pipefd[2]= {0},maxfdpl,sigval,cp;
    char c,cc,*hostname,cliuname,servname,cmdbuf[512];
    pid_t childpid;
    struct hostentry *hp= NULL;
    fd_set readfrom,ready,buf;
    struct passwd *pwd;
    char env_home[64], env_shell[64], env_user[64];
    
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		exit(1);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(13);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(connect(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))<0)
		exit(1);

    /*getpeername when Tcp is connected*/
	getpeername(sockfd, (struct sockaddr *)&cli_addr, sizeof(cli_addr));	
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char*)1, sizeof(1));
 	linger.l_onoff = 1;
	linger.l_linger = 60;
	setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

    /*Do(&cli_addr);*/
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);

    cli_addrp = malloc(sizeof(struct sockaddr_in));
    cli_addrp->sin_family = AF_INET;
	cli_addrp->sin_port = ntohs((ushort)cli_addr.sin_port);

    if(cli_addrp->sin_port>= IPPORT_RESERVED||
            cli_addrp->sin_port<= IPPORT_RESERVED/2)
    {
        /*alarm(10);*/
        clisecport = 0;
        printf("clisecport:\r\n");
	    for(;;)
        {
            if(cc= read(0, &c,1)!=1)
            {
                shutdown(0,2);
                printf("shutdown\r\n");
                exit(1);
            }
            else
            {
                clisecport= clisecport*10+(c-'0');
                break;
            }
        }
    }

    printf("clisecport %d\r\n", clisecport);
    if(clisecport!= 0)
    {   
        if(clisecport>= IPPORT_RESERVED)
            exit(1);
        oursecport = IPPORT_RESERVED-1;
                
        if(sockfd2= rresvport(&oursecport) <0)
            exit(1);

        cli_addrp->sin_family = AF_INET;
        cli_addrp->sin_port= htons(15);
        cli_addrp->sin_addr.s_addr = htonl(INADDR_ANY);
        if(connect(sockfd2, (struct sockaddr*)&cli_addrp, sizeof(cli_addrp))< 0)
        {
            printf("connect error\r\n");
            exit(1);         
        }
    }

    hp= gethostbyaddr((char *)&cli_addrp->sin_addr, sizeof(struct in_addr), cli_addrp->sin_family);
    setpwent();

    if(chdir(pwd->pw_dir)< 0)
        chdir("/");

    if(pwd->pw_passwd!=NULL && *pwd->pw_passwd!='\0'&&
        ruserok(hostname, pwd->pw_uid==0, cliuname,servname)<0)
        exit(1);
    
    if(pwd->pw_uid!=0&& access("/etc/nologin", F_OK)!=0)
        exit(1);
     
    /*write "" to terminal*/
    if(write(2,"s", 1)!=1)
        exit(1);    

    if(clisecport)
    {
        if(pipe(pipefd)< 0)
            exit(1);

        if(childpid=fork()== -1)
            exit(1);

        if(pipefd[0]>sockfd2)
            maxfdpl= pipefd[0];
        else
            maxfdpl= sockfd2;
        maxfdpl++;
    }   

    if(childpid!= 0)
    {
        printf("childpid run\r\n");
        close(0);
        close(1);
        close(2);
        close(pipefd[0]);

        FD_ZERO(&readfrom);
        FD_SET(sockfd2, &readfrom);
        FD_SET(pipefd[0], &readfrom);
        ioctl(pipefd, FIONBIO, (char*)1);

        /*Select允许进程指示内核等待多个事件中的某个发生，直至此事件发生或经过指定时间才唤醒进程*/
        if(select(maxfdpl, &ready, (fd_set *)0, 0, (struct timeval *)0)<0)
            exit(1);
        if(FD_ISSET(sockfd2, &ready))
        {            
            if(FD_ISSET(sockfd2, &readfrom)< 0)
                FD_CLR(sockfd2, &readfrom);
            else
                killpg(sockfd2, sigval);
        }

        FD_ISSET(pipefd[0], &ready);
        do
        {
            cc= read(pipefd[0]);
            if(cc< 0)
            {   
                shutdown(sockfd2, 2);
                FD_CLR(pipefd[0], &buf);
            }    
            else
                write(sockfd2, 2); 
        }while(FD_ISSET(sockfd2, &buf)||FD_ISSET(pipefd[0], &buf));
        /*move process to another group*/
        setpgrp(0, getpid());
        close(sockfd2);
        close(pipefd[0]);
        dup2(pipefd[1], 2);
        close(pipefd[1]);    
    }

    if(*pwd->pw_shell=='\0')
        pwd->pw_shell= "/bin/sh";
   
    setgid((gid_t)pwd->pw_shell);
    initgroups(pwd->pw_name, pwd->pw_gid);
    setuid((uid_t)pwd->pw_uid);
    //ebviron= env_prts;
    strncat(env_home, pwd->pw_dir, sizeof(env_home)-6);
    strncat(env_shell, pwd->pw_shell, sizeof(env_shell)-7);
    strncat(env_user, pwd->pw_name, sizeof(env_user)-6);

    if((cp=(rindex(pwd->pw_shell, "/")))!= NULL)
        cp++;
    else
        cp = pwd->pw_shell;
    execl(pwd->pw_shell, cp, "-c", cmdbuf, (char *)0);    
    
	return 0;
}

