#include<stdio.h>
#include<signal.h>
#include<setjmp.h>

jmp_buf position;
int catch();

int main(){

	setjmp(position);
	printf("setjmp\r\n");
	signal(SIGINT, catch);
	sleep(10);

	return;
}

int catch(){
	printf("catch\r\n");

	longjmp(position, 1);

	return;
}
