#include<stdio.h>  
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<termios.h>  
#include <termios.h>    
#include <unistd.h>     
#include <stropts.h> 

static char pty_name[12];

int main()
{
    int master_fd;
    
    pty_master();
    master_fd = open_master();
    if(master_fd< 0)
        return -1;
    
    if(open_slave(master_fd)<0)
        return -1;

    if(pty_slave()<0)
        return -1;
    else
        printf("\r\n");
    
    return 0;
}

int pty_master()
{
    int i, master_fd;
    char *ptr;
    struct stat statbuff;
    static char ptychar[] = "pqrs"; 
    static char hexdigit []= "0123456789abcdef"; 
    for(ptr=ptychar; *ptr!= 0; ptr++)
    {
        strcpy(pty_name, "dev/ptyXY");
        pty_name[8]= ptr ;          
        pty_name[9]= '0' ;
        if(stat(pty_name, &statbuff)<0)
            break;
    }
    
    for(i=0;i< 10; i++)
    {
        pty_name[9]= hexdigit[i]; /*0-15->0-9a-f */
        if (master_fd= open(pty_name,"rw")>=0)
            return master_fd;
    }
    return -1;
}

int pty_slave()
{
    int slave_fd;
    pty_name[5]= 't';
    if((slave_fd= open(pty_name, O_RDWR))<0)
    {
        close(slave_fd);
        return -1;
    }
}

#define PTY_MASTER "dev/ptmx"
int open_master()
{
    int master_fd;
    if(master_fd= open(pty_name, O_RDWR)<0)
        return -1;
    return master_fd;
}
    
int open_slave(int master_fd)
{
    int slave_fd;
    char *slavename;
    int grantpt(); /*libpt.a*/
    int nlockpt(); /*libpt.a*/
    char *ptsname(); /*libpt.a*/

    /*change dev right */
    if(grantpt(master_fd)<0)
        close(master_fd);
    if(unlockpt(master_fd)<0)
    {
        close(master_fd);
        return -1;
    }    
    slavename= ptsname(master_fd);
    if(slavename== NULL)
    {
        close(master_fd);
        return -1;
    }
    slave_fd = open(slavename, O_RDWR);
    if(slave_fd< 0)
    {
        close(master_fd);
        return -1;
    }
  
    if(ioctl(slave_fd, I_PUSH, "ptem")<0)
    {
        close(master_fd);
        return (-1);
    }
    
    return slave_fd;
}



