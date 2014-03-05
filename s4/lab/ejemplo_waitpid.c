#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


void trata_alarma(int s) 
{

}

int main(int argc,char *argv[])
{
	int pid,res;
	char buff[256];		
	int contador = 0;
	int hijos=0;
	for (hijos=0;hijos<10;hijos++)
	{
		sprintf(buff, "Creando el hijo numero %d\n", hijos);
		write(1, buff, strlen(buff)); 
		
		pid=fork();
		if (pid==0) /* Esta linea la ejecutan tanto el padre como el hijo */
		{
			signal (SIGALRM, trata_alarma);//si pusieramos esta linea fuera antes del fork, creo que solo se ejecutaria una unica vez
											//y ya quedaria gravado en el PCB.
			/* Escribe aqui el codigo del proceso hijo */
			sprintf(buff,"Hola, soy %d\n",getpid());
			write(1, buff, strlen(buff)); 

			alarm(1); 
			pause();

			/* Termina su ejecución */
			exit(0);
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

		if(pid > 0) {
			if(WIFEXITED(res)) sprintf(buff,"Termina el proceso %d con exito.\n",pid);
			if(WIFSIGNALED(res)) sprintf(buff,"Termina el proceso %d por un signal.\n",pid);		
		}
		//sprintf(buff,"Termina el proceso %d\n",pid);
		write(1, buff, strlen(buff)); 
		hijos --;
		contador++;
	}
	sprintf(buff,"Valor del contador %d\n", contador);
	write(1, buff, strlen(buff)); 
	return 0;
}
