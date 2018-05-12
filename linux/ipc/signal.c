#include<stdlib.h>
#include<stdio.h>
#include<signal.h>

int catch(),(*oldptr)();

int main()
{
	pid_t pid = fork();
	int status;

	if(pid > 0){
		printf("signal\r\n");
		//signal(SIGINT, SIG_IGN);
		oldptr = signal(SIGINT, catch);
		sleep(10);
		signal(SIGINT,oldptr);
		wait(&status);
		printf("return\r\n");
	}
	else if(pid == 0){
		wait(&status);
		printf("I am child, status %d\r\n", status);					
	}else{
		printf("error");
		exit(1);
	}

	return;
}

int catch(){
	printf("\nsignal callback\r\n");
	return;
}
