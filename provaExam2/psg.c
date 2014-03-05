/*
>psg userid
Mitjançant forks,pipes i execlp ha d'executar l'equivalent a la comanda:
$> ps -e -o user -no-heading|grep userid|wc -l
*/

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int muta_ps(int pipe1[2],int pipe2[2]) {
	//reedireccionamos el canal de salida estanda
	dup2(pipe1[1],1);
	close(pipe1[1]);
	//cerramos los canales que no utilizamos
	close(pipe1[0]);
	close(pipe2[0]);
	close(pipe2[1]);
	//mutamos
	execlp("ps","ps","-e","-o","user","-no-heading",(char *) NULL);
	exit(1);
}
int muta_grep(int pipe1[2],int pipe2[2],char* userid) {
	//reedireccionamos el canal de entrada estanda
	dup2(pipe1[0],0);
	close(pipe1[0]);	
	//reedireccionamos el canal de salida estanda	
	dup2(pipe2[1],1);
	close(pipe2[1]);
	//cerramos los canales que no utilizamos
	close(pipe1[1]);
	close(pipe2[0]);
	//mutamos
	execlp("grep","grep",userid,(char *) NULL);
	exit(1);
}
int muta_wc(int pipe1[2],int pipe2[2]) {
	//reedireccionamos el canal de entrada estanda
	dup2(pipe2[0],0);
	close(pipe2[0]);
	//cerramos los canales que no utilizamos
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[1]);
	//mutamos
	execlp("wc","wc","-l",(char *) NULL);
	exit(1);
}

void Usage() {
	fprintf(stderr, "Usage: psg userid");
    	exit(EXIT_FAILURE);
}

void error_y_exit(char *msg,int error_code) {
	perror(msg);
	exit(error_code);
}

int main(int argc, char *argv[])
{
	int pipefd1[2];//vector de los canales generados en el pipe para el hijo1='ps' y el hijo2='grep'
	int pipefd2[2];//vector de los canales generados en el pipe para el hijo2='grep' y el hijo3='wc'
	char buf[256];
	int pid,i;

	//control de error
	if (argc != 2) Usage();
	if (pipe(pipefd1) == -1) error_y_exit("pipe1",EXIT_FAILURE);
	if (pipe(pipefd2) == -1) error_y_exit("pipe2",EXIT_FAILURE);
	
	for(i = 0; i < 3; ++i) {
		switch(fork()) {
			case 0:
				if(i == 0) muta_ps(pipefd1,pipefd2);
				else if(i == 1) muta_grep(pipefd1,pipefd2,argv[1]);
				else muta_wc(pipefd1,pipefd2);
				break;
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);
				break;

		}
	}
	close(pipefd1[0]);close(pipefd1[1]);
	close(pipefd2[0]);close(pipefd2[1]);

	while(waitpid(-1,NULL,0)>0);

	sprintf(buf,"fi\n");
	write(1,buf,strlen(buf));
	
}
