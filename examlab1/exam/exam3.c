#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int aux = 0;

void Usage() {
	char buf[80];
	sprintf(buf,"exam3: argv[1] , 1 <= argv[1] <= 5 \n");
	write(1,buf,strlen(buf));
	exit(0);
}

void error_y_exit(char *msg,int exit_status) {
       perror(msg);
       exit(exit_status);
}


void muta_a_sleep()
{
  execlp("sleep", "sleep", "5", (char*)NULL);
  error_y_exit("Ha fallado la mutación al ps", 1);
}

int main(int argc,char *argv[])
{
    int pid,res;
    char buf[256];
    int contador = 0;
    int hijos=0;
    int i ,z= 0;
    if(argc <= 1) Usage();
    z = atoi(argv[1]);
    if(z < 1 || z > 5) Usage();
    for (hijos=0;hijos<z;hijos++)
    {        
        pid=fork();
        if (pid==0) /* Esta linea la ejecutan tanto el padre como el hijo */
        {   
            sprintf(buf,"HIJO:PID %d \n",getpid());
            write(1,buf,strlen(buf));
	    muta_a_sleep();
        }else if (pid<0) 
        {
            /* Se ha producido un error */
            perror("Error en el fork");
        }
    }
    /* Esperamos que acaben los hijos */
    while (hijos > 0)
    {
        pid=waitpid(-1,&res,0);
        sprintf(buf,"Termina el proceso %d\n",pid);
        write(1, buf, strlen(buf));
        hijos--;
        contador++;
    }
    sprintf(buf,"Proces pare finalitzat\n");
    write(1,buf,strlen(buf));
    return 0;
}
