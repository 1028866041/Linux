#include<stdio.h>
#include<stdlib.h>		
#include<string.h>	
#include<unistd.h>	
#include<signal.h>	
#include<fcntl.h>
#include<pthread.h>
#include<sys/resource.h>
#include<syslog.h>

sigset_t mask;

void *thr_fun(void *arg){
	int signo;

	for(;;){
		printf("pthread thr_fun\r\n");
		/*wait for process signal*/
		if(sigwait(&mask,&signo)<0){
			printf("sigwait error\r\n");
			exit(1);
		}
		
		switch(signo){
			case SIGHUP:
				printf("SIGHUP\r\n");
				break;
			case SIGTERM:
				printf("SIGTERM\r\n");
				break;
			default:
				syslog(LOG_INFO, "unexcepted signal %d\r\n",signo);
		}
		
	}

	return;
}

int main()
{
	pid_t pid;
	int i,fd0,fd1;
	struct rlimit rl;
	struct sigaction sa;
	pthread_t tid;

	umask(0);
	/*limit process useage*/
	if(getrlimit(RLIMIT_NOFILE, &rl)<0){
		printf("getrlimit error\r\n");
	}

	pid = fork();
	if(pid == 0){
		/*process change to group handler*/
		setsid();
		sa.sa_handler = SIG_DFL;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		
		if(sigaction(SIGHUP, &sa, NULL)<0){
			printf("sigaction error\r\n");	
		}
		sigfillset(&mask);

		if(chdir("/")<0)
			printf("chdir eror\r\n");

		/*close file*/
		if(rl.rlim_max==RLIM_INFINITY){
			rl.rlim_max = 1024;
		}

		printf("daemon\r\n");
		#if 0
		/*can't open IO when file close*/
		for(i=0;i<RLIM_INFINITY;i++)
			close(i);	
	
		fd0 = open("/dev/null", O_RDWR);	
		fd1 = dup(0);
		openlog("cmd",LOG_CONS, LOG_DAEMON);
		if(fd0 !=0 ||fd1 != 1){
			syslog(LOG_ERR,"unexception file descriptors %d %d",fd0,fd1);
			
			exit(1);
		}
		#else
			
		/*create thread then set thr_fun*/	
		if(pthread_sigmask(SIG_BLOCK,&mask,NULL)<0){
			printf("pthread_sigmask error\r\n");
			exit(1);
		}

		if(0 != pthread_create(&tid,NULL,thr_fun,0))
			printf("pthread_create error\r\n");
		else
			printf("create thread %lu\r\n",tid);
		/*process wait for slave thread to complete*/
		pthread_join(tid,NULL);
		exit(0);
		#endif
	}
	else if(pid>0){
		/*child go to backgroud*/
		exit(0);
	}
	else{
		exit(1);
	}

	return;	
}
