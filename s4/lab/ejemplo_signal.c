#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int hijos=0;

void trata_alarma(int s) 
{
	
}

void trata_waitpid(int s) {
	int pid,res;
	char buff[256];
	pid=waitpid(-1,&res,WNOHANG);
	if(pid > 0) {
		if(WIFEXITED(res)) sprintf(buff,"Termina el proceso %d con exito.\n",pid);
		if(WIFSIGNALED(res)) sprintf(buff,"Termina el proceso %d por un signal.\n",pid);		
	}
	write(1, buff, strlen(buff)); 
	hijos --;
}

void error_y_exit(char *msg,int error_status) {
	perror(msg);
	exit(error_status);
}
int main(int argc,char *argv[])
{
	int pid,res;
	char buff[256];		
	int contador = 0;
	
	for (hijos=0;hijos<10;hijos++)
	{
		sprintf(buff, "Creando el hijo numero %d\n", hijos);
		write(1, buff, strlen(buff)); 
		
		pid=fork();
		switch(fork()) {
			case 0: //proceso hijo
				/*signal(SIGALRM,trata_alarma);
				alarm(1);*/
				signal (SIGALRM, trata_alarma);//si pusieramos esta linea fuera antes del fork, creo que solo se ejecutaria una unica vez
											//y ya quedaria gravado en el PCB.
				/* Escribe aqui el codigo del proceso hijo */
				sprintf(buff,"Hola, soy %d\n",getpid());
				write(1, buff, strlen(buff)); 

				alarm(1); 
				pause();

				/* Termina su ejecución */
				exit(0);
				break;
			case -1: //erro en crear el proceso
				error_y_exit("error en crear fork()",1);
				break;
			default: //proceso padre
				signal(SIGCHLD,trata_waitpid);
		}
	}
	/* Esperamos que acaben los hijos */
	while (hijos > 0)
	{
		contador++;
	}
	sprintf(buff,"Valor del contador %d\n", contador);
	write(1, buff, strlen(buff)); 
	return 0;
}
