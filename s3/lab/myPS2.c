#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void Usage() {
	char buf[80];
	sprintf(buf,"Usage: argv[1] (argv[2]....argv[n])");
	write(1,buf,strlen(buf));
	exit(0);
}

void error_y_exit(char *msg,int exit_status) {
       perror(msg);
       exit(exit_status);
}

/* Ejecuta el comando ps –u username mediante la llamada al sistema execlp */
/* Devuelve: el código de error en el caso de que no se haya podido mutar */
void muta_a_PS(char *username)
{
  execlp("ps", "ps", "axo", "state,ppid,pcpu", (char*)NULL);
  error_y_exit("Ha fallado la mutación al ps", 1);
}


int main(int argc, char *argv[]) {
	int pid;
	char buf[80];
	int i;
	int status;
	if(argc <= 1) Usage();
	for(i = 1; i < argc; ++i) {
		pid = fork();
		switch(pid) {
			case 0:
			//codigo hijo
				sprintf(buf,"print hijo,PID: %d --- User: %s\n",getpid(),argv[i]);
				write(1,buf,strlen(buf));
				muta_a_PS(argv[i]);
				exit(0);
				break;
			case -1:
			//codigo error
				error_y_exit("Error en fork",1);
				break;
			default:
			//codigo padre
				sprintf(buf,"print padre, PID: %d\n",getpid());
				write(1,buf,strlen(buf));
				//while(1);
		}
		waitpid(pid,&status,0);
	}		
}
