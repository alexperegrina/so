#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void error_y_exit(char* msg,int exitStatus) {
	perror(msg);
	exit(exitStatus);
}

void muta_ps() {
	//execlp("ps","cat","-eo","pid:5","--no-header",NULL);
	execlp("ps","ps","-eo","pid:5","--no-header",NULL);
	error_y_exit("a fallado la mutacion\n",1);
}

void Usage() {
	printf ("Usage(): ./exam4.c namefile\n");
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc != 2) Usage();
	 char final[32];
	sprintf(final, "%s.txt", argv[1]);
	char buf[6];
	char c[2];
	sprintf(c,"\n");
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
		int f = creat(final,O_CREAT|O_TRUNC|0600);
		if(f == -1) error_y_exit("error en crear el archivo",1);
		close(fd[1]);
		int n = 0;
		int cont = 0;
		int num;
		while(n = read(fd[0],buf,6)>0){
			++cont;
			num = atoi(buf);
			write(f,&num,sizeof(num));
			write(f,c,strlen(c));
		}
		close(fd[0]);
		waitpid(-1,NULL,0);
	}
}
