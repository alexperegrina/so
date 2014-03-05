#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* ESTA VARIABLE SE ACCEDE DESDE LA FUNCION DE ATENCION AL SIGNAL Y DESDE EL MAIN */
int segundos=0;

void Usage() {
	char buf[80];
	sprintf(buf,"exam4: argv[1] , 1 <= argv[1] <= 5 \n");
	write(1,buf,strlen(buf));
	exit(0);
}

void error_y_exit(char *msg,int exit_status) {
       perror(msg);
       exit(exit_status);
}

/* FUNCION DE ATENCION AL SIGNAL SIGALRM */
void funcion_alarma(int s)
{
        char buff[256];
        if (s == SIGUSR1)
        {
	    //aqui consultamos el tiempo y mostramos por pantalla

        }
 
}

int main(int argc, char *argv[]) {
	int pid;
	char buf[80];
	int i,z;
	int status;
	if(argc <= 1) Usage();
	z = atoi(argv[1]);
	if(z < 1 || z > 5) Usage();
	for(i = 0; i < z; ++i) 
	{
		pid = fork();
		switch(pid) {
			case 0:
			//codigo hijo
				signal(SIGUSR1,funcion_alarma);
				sprintf(buf,"HIJO:PID %d \n",getpid());
				write(1,buf,strlen(buf));
				sleep(5);
				sprintf(buf,"Poces %d finalitzat\n",getpid());
				write(1,buf,strlen(buf));
				exit(0);
				break;
			case -1:
			//codigo error
				error_y_exit("Error en fork",1);
				break;
		}
		
	}
	while(waitpid(-1,NULL,0) > 0);
	sprintf(buf,"Proces pare finalitzat\n");
	write(1,buf,strlen(buf));
	
}
