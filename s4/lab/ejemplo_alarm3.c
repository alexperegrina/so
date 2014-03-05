#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int segundos=0;
void funcion_alarma(int signal)
{
	char buff[256];
	segundos=segundos+10;
	sprintf(buff,"ALARMA pid=%d: %d segundos\n",getpid(),segundos);
	write(1, buff, strlen(buff) );
}

void error_y_exit(char *msg,int exit_status) {
	perror(msg);
	exit(exit_status);
}

void muta_a_bucle() {
	execlp("./bucle","bucle",(char*)NULL);
	error_y_exit("Ha fallado la mutacion al bucle",1);
}

int main (int argc,char * argv[])
{
	
	switch(fork()) {
		case 0: //proceso hijo
			signal(SIGALRM,funcion_alarma);
			break;
		case -1: //erro en crear el proceso
			error_y_exit("error en crear fork()",1);
			break;
		default: //proceso padre
			signal(SIGALRM,funcion_alarma);
	}
	while (segundos<100)
	{
		alarm(10);
		muta_a_bucle();
		pause();
	}
	exit(1);
}
