#include<stdio.h>
#include<string.h>
#include<sys/types.h>	
#include<sys/socket.h>	
#include<sys/time.h>	
#include<netinet/in.h>	
#include<arpa/inet.h>

char buf[1025];
int main(){	
	int flag = 0;

	printf("service\r\n");
	fflush(stdout);
	
	do{
		while(fgets(buf,1024, stdin)==NULL);
		
		flag=1;
		switch(getcmd(buf)){
			case 0:
				printf("match hello\r\n");
				break;
			case 1:
				printf("match bye\r\n");
				break;
			case 2:
				printf("match good\r\n");
				break;
			default:
				printf("miss match\r\n");
				flag=0;
				break;
		}
		
		if(strncasecmp(buf,"exit",4)==0){
			flag =1;
		}
		//printf("input is %s",buf);
	}while(!flag);
		
	fflush(stdout);
	
	return;
}

char *cmds[] ={"hello","bye","good",NULL};
int getcmd(char * cmd){
	
	int i=0;		
	while(cmds[i] !=NULL){
		if(strncasecmp(cmd, cmds[i], strlen(cmds[i]))==0)
			return i;
		i++;
	}

	return -1;
}
