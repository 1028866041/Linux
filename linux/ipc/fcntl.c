#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<stropts.h>
#include<sys/ioctl.h>
#include<sys/types.h>

char buf[1024];

int main(int argc,char **argv){

	int i,fd;
	pid_t pid;
	char *ptr;
	int ntowrite,nwrite;

	for(i=1;i<argc;i++){
		if(fd = open(argv[i],O_RDONLY)<0){
			printf("open error\r\n");	
			continue;
		}
		if(ioctl(fd,I_CANPUT,0)!= -1)
			printf("is stream\r\n");
		else
			printf("not stream\r\n");
		close(fd);
	}
#if 0	
	pid = fork();
	if(pid>0){
		if(lockbyte("parent",fd,1)<0){
			printf("lockbyte error\r\n");
		}
		printf("parent %d\r\n",getpid());
	}
	else if(pid==0){
		if(lockbyte("child",fd,1)<0){
			printf("lockbyte error\r\n");
		}
		printf("child %d\r\n",getpid());
	}else{
		printf("fork error\r\n");
		exit(1);
	}
#endif

	ntowrite = read(STDIN_FILENO,buf,sizeof(buf));
	if(fcntl(STDOUT_FILENO,F_SETFL,
				(fcntl(STDOUT_FILENO,F_GETFL,0)|O_NONBLOCK))<0){
		printf("set fd noblocking error\r\n");
		return;
	}
	fprintf(stderr,"read %d bytes\n",ntowrite);
	for(ptr=buf;ntowrite>0;){
		nwrite = write(STDOUT_FILENO,ptr,ntowrite);
		if(nwrite>0){
			ptr+=nwrite;
			ntowrite -= nwrite;
		}
		printf("file write %d left %d\r\n",nwrite,ntowrite);
	}
	if(fcntl(STDOUT_FILENO,F_SETFL,
				(fcntl(STDOUT_FILENO,F_GETFL,0)&(~O_NONBLOCK)))<0){
		printf("clear fd noblocking error\r\n");
		return;
	}
	return;
}

int lockbyte(char *name,int fd,off_t offset){
	struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_start = offset;
	lock.l_whence = SEEK_SET;
	lock.l_len = 1;
	#if 0
	/*get fcntl lock*/
	if(fcntl(fd,F_GETLK, &lock)<0){
		printf("flock error\r\n");
	}
	if(lock.l_type == F_UNLCK){
		printf("flock F_UNLCK\r\n");
		return 0;
	}
	#else
	/*F_SETLKW blocking lock*/
	if(fcntl(fd, F_SETLKW, &lock)<0){
		printf("fcntl F_SETLKW error\r\n");
		return -1;
	}
	#endif
	return;
}

