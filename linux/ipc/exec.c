#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

int interupt();
int catch();

#define LEN 256

int main(){
	pid_t pid;
	int fd;
	char *buf = (char *)malloc(LEN);
	char *argv[]={"ls","-a",NULL};

	fd = open("${pwd}/a.txt, O_RDWR);
	if(fd < 0)
		perror("open erorr");

	pid = fork();
	if(pid > 0){
		printf("I am parent\r\n");
		
		signal(SIGQUIT,catch);
		signal(SIGINT,interupt);
		
		if(write(tfd,buf,SIZE)<0){
			if(){
				perror();
			}
		}
		
		sleep(10);
	
		printf("nromal\r\n");
		execlp("pwd", "-option", (char *)0);
		
		exit(0);
	}
	else if(pid == 0){
		/*child process couldn't catch signal directly*/
		printf("I am child\r\n");
		execv("/bin/ls",argv);	
		//exit(0);
	}
	else{
		printf("fork error\r\n");
		exit(1);
	}
	
	return;
}

int interupt(){
	
	printf("interupt\r\n");
	/*signal stores if function isn't finish*/
	sleep(2);

	return;
}

int catch(){

	printf("ctrl+ catch\r\n");
	return;
}

