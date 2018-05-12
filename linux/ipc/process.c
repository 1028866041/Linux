#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#define LEN 256
int main(){
	char *buf = (char *)malloc(LEN);
	int fd = 0;

	fd = open("/bin/sh", O_RDONLY);
	if(fd < 0)
		perror("open erorr");
	read(fd,buf,LEN);
	printf("fcntl %d\r\n",fcntl(fd,F_SETFD,1));

	pid_t pid = vfork();
	if(pid > 0){
		printf("I am parent\r\n");
		printpos(fd,"parent");
	
		pid_t pidt= vfork();
		switch(pidt){
		    case 0:
		        printf("in clild\r\n");
				execl("/bin/ls","ls","-l",NULL);
				perror("in child error\r\n");
				break;
			case -1:
				printf("in failed\r\n");
				break;
			default:
				printf("in parent\r\n");
				execl("/bin/pwd","pwd",NULL);
				perror("in parent error\r\n");
				exit(0);
		}
	}
	else if(pid == 0){
		printf("I am child\r\n");
		read(fd,buf,LEN);
		printpos(fd,"child");
		execl("/bin/ls","-a",NULL);
		exit(0);
	}else{
		printf("process failed\r\n");
	}
	close(fd);
	
	return;
}

int printpos(int fd,char *str)
{
	long pos;
	
	if((pos = lseek(fd,0,SEEK_CUR)) < 0)
		perror("lseek error");	
	printf("lseek:%s,%lu\r\n", str,pos);
	return;	
}

