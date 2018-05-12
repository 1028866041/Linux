#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main(){

	int fd[2],bytes;
	pid_t pid;
	char str[] = "pipe";
	char buf[64] = {0};
	
	pipe(fd);
	pid = fork();
	/*If using vfork then parent woun't read pipe*/
	if(pid == 0){
		printf("child\r\n");
		write(fd[1],str, strlen(str));
		close(fd[0]);
	}
	else if(pid >0){
		close(fd[1]);
		read(fd[0],buf, sizeof(buf));
		printf("parent %d: %s\r\n",getpid(),buf);
		exit(0);
	}
	else{
		printf("fork error");
		exit(1);
	}

	



	return;
}
