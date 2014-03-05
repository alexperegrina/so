#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void error_y_exit(char* msg,int exitStatus) {
	perror(msg);
	exit(exitStatus);
}

void muta_ps() {
	//execlp("ps","cat","-eo","pid:5","--no-header",NULL);
	execlp("ps","ps","-eo","pid:5","--no-header",NULL);
	error_y_exit("a fallado la mutacion\n",1);
}

int main() {
	char buf[6];
	int fd[2];
	if(pipe(fd) == -1) error_y_exit("a fallado la creacion del pipe\n",1);
	int pid = fork();
	if(pid == -1) error_y_exit("error en el fork\n",1);
	if(pid == 0) { //hijo
		dup2(fd[1],1);
		close(fd[0]);close(fd[1]);
		muta_ps();
	}
	else { //padre
		close(fd[1]);
		int n = 0;
		int cont = 0;
		while(n = read(fd[0],buf,6)>0){
			++cont;
		}
		close(fd[0]);
		waitpid(-1,NULL,0);
		sprintf(buf,"%d\n",cont);
		write(1,buf,strlen(buf));
	}
}
