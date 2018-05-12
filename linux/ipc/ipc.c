#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<stdlib.h>

struct msgbuf{
	long type;
	long request;
	double data;
};

int main(){

	int qid,shmid;
	char *ptr,*shmptr;
	key_t msgkey;
	struct msgbuf msg;

	/*create IPC msg by id*/
	msgkey = ftok(".", 'm');

	if((qid =msgget(msgkey))<0){
		printf("msgget failed\r\n");
		return -1;
	}

	/*fill msg data*/
	msg.type = 1;
	msg.request = 1;
	msg.data = 10240;

	for(;;){
		if(msgsnd(qid, &msg,sizeof(struct msgbuf)-sizeof(long),0)==-1)
			printf("msgsnd error\r\n");

		if(msgrcv(qid, &msg,sizeof(struct msgbuf)-sizeof(long),0)!=-1){
			printf("msgrcv qid %d success\r\n",qid);
			break;
		}else{
			printf("msgrcv error\r\n");
			exit(1);
		}
	}

	/*read msg data then store*/
	if(msgctl(qid,IPC_STAT,&msg)==-1)
		printf("msgctl stat error\r\n");

	#if 0
	/*couldn't delete msg before msgrcv break*/
	if(msgctl(qid,IPC_RMID,0)==-1){
		printf("msgctl delete error\r\n");
		return -1;
	}
	#endif

	if((shmid = shmget(IPC_PRIVATE,10240,0600/*usr RW*/))<0)
		printf("shmget error\r\n");

	if((shmptr=shmat(shmid,0,0)) == (void *)-1)
		printf("shmat error\r\n");

	if(shmctl(shmid,IPC_RMID,NULL)<0)
		printf("shmctl error\r\n");

	printf("shared memory from %lu to %lu \r\n", 
			(unsigned long)shmptr,(unsigned long)shmptr+10240);

	exit(0);
}
