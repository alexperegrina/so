#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

void error_y_exit(char* msg,int exitStatus) {
	perror(msg);
	exit(exitStatus);
}

void muta_ps() {
	execlp("ps","ps","-eo","pid:5","--no-header",NULL);
	error_y_exit("a fallado la mutacion\n",1);
}

void muta_wc() {
	execlp("wc","wc","-l",NULL);
	error_y_exit("a fallado la mutacion\n",1);
}

int main() {
	char buf[6];
	int fd2[2];//comunicacion wc-padre
	if(pipe(fd2) == -1) error_y_exit("a fallado la creacion del pipe\n",1);
	int pid = fork();
	if(pid == -1) error_y_exit("error en el fork\n",1);
	if(pid == 0) { //hijo	
		int n2 = mknod("pipeAB", S_IFIFO|0666,0);
		if((n2 == -1 && errno == EEXIST) || n2 != -1) {
			int pid2 = fork();
			if(pid2 == -1) error_y_exit("error en el fork\n",1);
			if(pid2 == 0) { //proceso wc

				int fp = open("pipeAB",O_RDONLY);
				close(fd2[0]);
				dup2(fp,0);close(fp);//redireccionamos la entrada estandar
				dup2(fd2[1],1);close(fd2[1]);//redireccionamos la salida estandar
				muta_wc();
			}
			else { //proceso ps
				
				int fp = open("pipeAB",O_WRONLY);
				close(fd2[0]);close(fd2[1]);
				dup2(fp,1);close(fp);//redireccionamos la salida estandar
				muta_ps();		
			}
		}
	}
	else { //padre
		close(fd2[1]);
		int n = 0;
		n = read(fd2[0],buf,6);
		if(n == -1) error_y_exit("a fallado la lectura\n",1);
		close(fd2[0]);
		while(waitpid(-1,NULL,0)>0)
		n = write(1,buf,n);
		if(n == -1) error_y_exit("a fallado la escritura\n",1);
	}
}
